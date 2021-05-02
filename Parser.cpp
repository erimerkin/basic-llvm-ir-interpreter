#include "Parser.hpp"
#include <iostream>

int ASTNode::tempIndex = 0;
int ConditionalNode::conditionalIndex = 0;
int ChooseNode::chooseIndex = 0;

/**
 * Parses the given grammar
 * (<expression>)
 * */
ASTNode *Parser::parseParanExpr()
{
    currentToken = getToken();

    //Parses inside of parantheses
    ASTNode *node = parseExpr();

    //Checks if existence of closing bracket
    if (currentToken.value != ")")
    {
        syntaxError(line);
        return nullptr;
    }

    //Gets new token
    currentToken = getToken();

    return node;
}

/**
 * Parses the given choose function
 * choose"("<expression>"," <expression"," <expression>"," <expression>")"
 * if expr1>0 returns expr2, else if expr
 * */
ASTNode *Parser::parseChoose()
{
    cout << line << "\n";
    currentToken = getToken();

    if (currentToken.value == "(")
    {
        currentToken = getToken();
        ASTNode *expr1 = parseExpr();
        if (currentToken.value == ",")
        {
            currentToken = getToken();
            ASTNode *expr2 = parseExpr();
            if (currentToken.value == ",")
            {
                currentToken = getToken();
                ASTNode *expr3 = parseExpr();
                if (currentToken.value == ",")
                {
                    currentToken = getToken();
                    ASTNode *expr4 = parseExpr();
                    if (currentToken.value == ")")
                    {
                        cout << "testi\n";
                        return new ChooseNode(expr1, expr2, expr3, expr4);
                    }
                }
            }
        }
    }

    cout<<line<< "\n";
    syntaxError(line);
    return nullptr;
}

/**
 * Parses the given grammar
 * <factor> ::= <identifier> | <integer> | "("<expression>")"|
 * choose"("<expression>"," <expression>"," <expression>"," <expression>")"
 * */
ASTNode *Parser::parseFactor()
{

    //Checks if the grammar is <factor> ::= (<expression>)
    if (currentToken.value == "(")
    {
        //Parses given grammar
        return parseParanExpr();
    }

    ASTNode *node = nullptr;

    switch (currentToken.type)
    {
    case (token_choose):
        node = parseChoose();
        currentToken = getToken();

        return node;
    case (token_number):
        node = new NumberNode(currentToken.value);
        currentToken = getToken();
        return node;
    case (token_identifier):
        variables.insert(currentToken.value);
        node = new IdentifierNode(currentToken.value);
        currentToken = getToken();
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
    if (node == nullptr)
    {
        return nullptr;
    }

    while (currentToken.value == "*" || currentToken.value == "/")
    {
        string opSign = currentToken.value;
        currentToken = getToken();
        ASTNode *expr = parseFactor();
        if (expr == nullptr)
        {
            return nullptr;
        }
        node = new BinaryOperationNode(node, expr, opSign[0]);
    }

    return node;
}

ASTNode *Parser::parseExpr()
{
    ASTNode *node = parseTerm();

    if (node == nullptr)
    {
        return nullptr;
    }

    while (currentToken.value == "+" || currentToken.value == "-")
    {
        string opSign = currentToken.value;
        currentToken = getToken();
        ASTNode *expr = parseTerm();
        if (expr == nullptr)
        {
            return nullptr;
        }
        node = new BinaryOperationNode(node, expr, opSign[0]);
    }
    return node;
}

ASTNode *Parser::parsePrint()
{
    PrintNode *node = nullptr;
    currentToken = getToken();

    if (currentToken.value == "(")
    {
        node = new PrintNode(parseParanExpr());
        return node;
    }

    syntaxError(line);
    return node;
}

ASTNode *Parser::parseStatement()
{
    string value = currentToken.value;

    switch (currentToken.type)
    {
    case (token_eol):
        currentToken = getToken();
        return nullptr;
    case (token_identifier):
        currentToken = getToken();
        AssignNode *node;

        if (currentToken.value == "=")
        {

            IdentifierNode *id = new IdentifierNode(value);

            currentToken = getToken();

            this->variables.insert(value);
            node = new AssignNode(id, parseExpr());
            return node;
        }
        syntaxError(line);
        return nullptr;
    case (token_print):
        return parsePrint();
    default:
        return parseExpr();
    }
}

ASTNode *Parser::parse()
{
    if (currentToken.value == "")
    {
        currentToken = getToken();
    }
    ASTNode *node = nullptr;

    switch (currentToken.type)
    {
    case (token_eof):
        return nullptr;
    case (token_eol):

        currentToken = getToken();

        return parse();
        break;
    case (token_conditional):
        int type;

        if (currentToken.value == "if")
        {
            type = 0;
        }
        else if (currentToken.value == "while")
        {
            type = 1;
        }
        else
        {
            syntaxError(line);
            return nullptr;
        }

        ConditionalNode *cond;

        currentToken = getToken();
        if (currentToken.value == "(")
        {

            ASTNode *condition = parseParanExpr();

            cond = new ConditionalNode(type, condition);

            if (currentToken.type == token_eol)
            {
                cout << "eol\n";
                currentToken = getToken();
            }
            cout << "test: " << currentToken.value << "\n";

            if (currentToken.value == "{")
            {

                currentToken = getToken();
                cout << "test: " << currentToken.value << "\n";

                if (currentToken.type == token_eol)
                {
                    currentToken = getToken();
                }
                //Parse expressions till there is a '}' symbol found
                while (currentToken.value != "}" && !this->error)
                {
                    ASTNode *temp = parseStatement();

                    if (temp != nullptr)
                    {
                        cond->statements.push_back(temp);
                    }

                    //There is no '}' file ended. syntax error
                    if (currentToken.type == token_eof)
                    {
                        syntaxError(line);
                        return nullptr;
                    }
                }
                currentToken = getToken();
                return cond;
            }
        }

        syntaxError(line);
        return nullptr;
    default:
        node = parseStatement();
        return node;
    }
}

Parser::Parser(Tokenizer *_tokenizer)
{
    this->tokenizer = _tokenizer;
    this->error = false;
}

void Parser::syntaxError(int line)
{
    cout << "errroorrr!!\n";
    this->error = true;
    this->line = line;
}

Token Parser::getToken()
{
    Token tok = tokenizer->getNextToken();
    this->line = tokenizer->line;
    return tok;
}