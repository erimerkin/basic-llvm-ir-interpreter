#include "Parser.h"

ASTNode *Parser::parseParanExpr()
{
    ASTNode *node = parseExpr();

    currentToken = tokenizer->getNextToken();

    if (currentToken->value != ")")
    {
        syntaxError(line);
        return nullptr;
    }

    return node;
}

ASTNode *Parser::parseChoose()
{
    ChooseNode *node;

    if (currentToken->value == "(")
    {
        node->expr1 = parseExpr();
        if (currentToken->value == ",")
        {
            node->expr2 = parseExpr();
            if (currentToken->value == ",")
            {
                node->expr3 = parseExpr();
                if (currentToken->value == ",")
                {
                    node->expr4 = parseExpr();
                    if (currentToken->value == ")")
                    {
                        return node;
                    }
                }
            }
        }
    }
    syntaxError(line);
    return nullptr;
}

ASTNode *Parser::parseFactor()
{
    if (currentToken->value == "(")
    {
        return parseParanExpr();
    }

    ASTNode *node = nullptr;

    switch (currentToken->type)
    {
    case (token_choose):
        node = parseChoose();
        currentToken = tokenizer->getNextToken();
        return node;
    case (token_number):
        node = new NumberNode(currentToken->value);
        currentToken = tokenizer->getNextToken();
        return node;
    case (token_identifier):
        variables.insert(currentToken->value);
        node = new IdentifierNode(currentToken->value);
        currentToken = tokenizer->getNextToken();
        return node;
    default:
        //if the token is not any of the given it should be syntax error
        syntaxError(line);
        return node;
    }
}

ASTNode *Parser::parseTerm()
{
    ASTNode *node = parseFactor();

    while (currentToken->value == "*" || currentToken->value == "/")
    {
        string opSign = currentToken->value;
        currentToken = tokenizer->getNextToken();
        ASTNode *expr = parseFactor();

        node = new BinaryOperationNode(node, expr, opSign[0]);
    }

    return node;
}

ASTNode *Parser::parseExpr()
{

    ASTNode *node = parseTerm();

    while (currentToken->value == "+" || currentToken->value == "-")
    {
        string opSign = currentToken->value;
        currentToken = tokenizer->getNextToken();
        ASTNode *expr = parseTerm();

        node = new BinaryOperationNode(node, expr, opSign[0]);
    }

    return node;
}

ASTNode *Parser::parsePrint()
{
    PrintNode *node = nullptr;
    currentToken = tokenizer->getNextToken();

    if (currentToken->value == "(")
    {
        node = new PrintNode(parseParanExpr());
    }

    syntaxError(line);

    return node;
}

ASTNode *Parser::parseStatement()
{

    if (currentToken->type == token_print)
    {
        return parsePrint();
    }
    else if (currentToken->type == token_identifier)
    {
        ASTNode *id = parseFactor();
        currentToken = tokenizer->getNextToken();

        if (currentToken->value == "=")
        {
        }
    }
    else
    {
        syntaxError(line);
        return nullptr;
    }
}

ASTNode *Parser::parse()
{
    currentToken = tokenizer->getNextToken();

    switch (currentToken->type)
    {
    case (token_eol):
        currentToken = tokenizer->getNextToken();
    case (token_conditional):
        int type;

        if (currentToken->value == "if")
        {
            type = 0;
        }
        else if (currentToken->value == "while")
        {
            type = 1;
        }
        else
        {
            syntaxError(line);
            return nullptr;
        }

        ConditionalNode *node = nullptr;

        currentToken = tokenizer->getNextToken();
        if (currentToken->value == "(")
        {
            node = new ConditionalNode(type, parseParanExpr());

            currentToken = tokenizer->getNextToken();

            if (currentToken->value == "{")
            {
                //Parse expressions till there is a '}' symbol found
                while (currentToken->value != "}")
                {
                    node->statements.push_back(parseExpr());

                    //There is no '}' file ended. syntax error
                    if (currentToken->type == token_eof)
                    {
                        syntaxError(line);
                        return nullptr;
                    }
                }
                return node;
            }
        }

        syntaxError(line);
        return nullptr;

    default:
        ASTNode *node = parseStatement();
        return node;
    }
}

void Parser::syntaxError(int line){
    this->error = true;
    this->line = line;
}