#include "Parser.hpp"

int ASTNode::tempIndex = 0;
int ConditionalNode::conditionalIndex = 0;
int ChooseNode::chooseIndex = 0;

/**
 * Constructor for Parser class
 * */
Parser::Parser(Tokenizer *_tokenizer)
{
    this->tokenizer = _tokenizer;
    this->error = false;
}

/**
 * Sets variables for syntax error
 * */
void Parser::syntaxError(int line)
{
    this->error = true;
    this->errLine = line;
}

/**
 * Gets tokens from tokenizer and adds the current line index of input file 
 * */
Token Parser::getToken()
{
    Token tok = tokenizer->getNextToken();
    this->line = tokenizer->line;
    return tok;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//// GRAMMAR PARSE FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////////////

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
        return NULL;
    }

    //Gets new token
    currentToken = getToken();

    return node;
}

/**
 * Parses the given choose function with recursive descent
 * <choose> ::= "choose("<expression>"," <expression"," <expression>"," <expression>")"
 * */
ASTNode *Parser::parseChoose()
{
    currentToken = getToken(); //gets next token

    //checks if it is ( otherwise throws syntax error
    if (currentToken.value == "(")
    {
        currentToken = getToken(); //gets next token
        ASTNode *expr1 = parseExpr(); //parse first <expression>

        if (currentToken.value == ",") //checks if "," exists, otherwise throw syntax error
        {
            currentToken = getToken();
            ASTNode *expr2 = parseExpr(); //parse second <expression>

            if (currentToken.value == ",") //checks if "," exists, otherwise throw syntax error
            {
                currentToken = getToken();
                ASTNode *expr3 = parseExpr(); //parse third <expression>

                if (currentToken.value == ",") //checks if "," exists, otherwise throw syntax error
                {
                    currentToken = getToken(); //gets next token
                    ASTNode *expr4 = parseExpr(); //parse forth <expression>

                    if (currentToken.value == ")") //checks if ")" exists, otherwise throw syntax error
                    {
                        return new ChooseNode(expr1, expr2, expr3, expr4);
                    }
                }
            }
        }
    }

    syntaxError(line);
    return NULL;
}

/**
 * Parses the given grammar
 * <factor> ::= <identifier> | <integer> | "("<expression>")"| <choose>
 * */
ASTNode *Parser::parseFactor()
{

    //Checks if the grammar is <factor> ::= (<expression>)
    if (currentToken.value == "(")
        return parseParanExpr(); //Parses given grammar

    ASTNode *node = NULL;

    //Checks token types
    switch (currentToken.type)
    {
    case (token_choose):           //<factor> ::= <choose>
        node = parseChoose();      //parses <choose>
        currentToken = getToken(); //gets next token
        return node;

    case (token_number):                           //<factor> ::= <integer>
        node = new NumberNode(currentToken.value); //Creates ASTNode for <integer>
        currentToken = getToken();                 //gets next token
        return node;

    case (token_identifier):                  //<factor> ::= <identifier>
        variables.insert(currentToken.value); //pushes the variable if it is not located there

        node = new IdentifierNode(currentToken.value); //creates ASTNode for <identifier>
        currentToken = getToken();                     //gets next token
        return node;

    default: //if the token is not any of the given it should be syntax error
        syntaxError(line);
        return node;
    }
}

/**
 * Parses given <term> grammar with recursive descent
 * <term> ::= <factor> "*" <term> | <factor> "/" <term> | <factor>
 * */
ASTNode *Parser::parseTerm()
{
    //Parses the left side <factor>
    ASTNode *node = parseFactor();

    //Left side parse is not successful, exit function
    if (node == NULL)
        return node;

    //Checks if the operator is "*" or "*" and recursively parses <factor> on right side of operators
    //this creates a tree as it calls and stores lower functions recursively
    while (currentToken.value == "*" || currentToken.value == "/")
    {
        string opSign = currentToken.value; //Store operation sign
        currentToken = getToken();          //gets next token

        ASTNode *expr = parseFactor(); //Parse right side of operator for <factor>

        //If parsing right side parse is not successfull exit function
        if (expr == NULL)
            return NULL;

        //create operation node and assign it to node
        node = new BinaryOperationNode(node, expr, opSign[0]);
    }

    return node;
}

/**
 * Parses given <expression> grammar with recursive descent
 * <expression> ::= <term> "+" <expression> | <term> "-" <expression> | <term>
 * */
ASTNode *Parser::parseExpr()
{
    //Parses the left side <term>
    ASTNode *node = parseTerm();

    //Left side parse is not successful, exit function
    if (node == NULL)
        return node;

    //Checks if the operator is "+" or "-" and recursively parses right side of operators
    //this creates a tree as it calls and stores lower functions recursively
    while (currentToken.value == "+" || currentToken.value == "-")
    {
        string opSign = currentToken.value; //Store operation sign
        currentToken = getToken();          //get next token

        ASTNode *expr = parseTerm(); //parse the right side

        //Right side parse is not successfull exit function
        if (expr == NULL)
            return NULL;

        //create operation node
        node = new BinaryOperationNode(node, expr, opSign[0]);
    }
    return node;
}

/**
 * Parses the given <print> grammar by recursive descent
 * <print> ::= "print("<expression>")"
 * */
ASTNode *Parser::parsePrint()
{
    currentToken = getToken(); //gets next token

    if (currentToken.value == "(") //checks if ( exists otherwise throw error
    {
        return new PrintNode(parseParanExpr()); //parse the expression inside parentheses
    }

    syntaxError(line);
    return NULL;
}

/**
 * Parses given <statement> grammar with recursive descent
 * <statement> ::= <print> | <identifier> '=' <expression> | <expression>
 * */
ASTNode *Parser::parseStatement()
{
    string value = currentToken.value;

    //
    switch (currentToken.type)
    {
    case (token_eol): //if end of line get new token and return
        currentToken = getToken();
        return NULL;

    case (token_identifier): // if type is identifier it may be assignment statement

        currentToken = getToken(); //get next token

        if (currentToken.value == "=") //checks if it is assignment otherwise throw error
        {
            IdentifierNode *id = new IdentifierNode(value); //create ID node for left side
            this->variables.insert(value);                  //add to the variable list if it doesn't exist

            currentToken = getToken(); //get next token

            return new AssignNode(id, parseExpr()); //parse right side of '='
        }

        syntaxError(line);
        return NULL;

    case (token_print): //if the type is print statement parse it
        return parsePrint();
    default: //default case is it is a expression
        return parseExpr();
    }
}

/**
 * Parses the given <program> grammar by recursive descent
 * <program> ::= <statement> | if '('<expression>')' then { <statement> } end 
 *              | while '('<expression>')' then { <statement> } end
 * */
ASTNode *Parser::parse()
{
    //Checks if currentToken is empty
    if (currentToken.value == "")
    {
        currentToken = getToken(); //If empty get new
    }

    //Checks token type
    switch (currentToken.type)
    {
    case (token_eof): //End of file exit
        return NULL;
    case (token_eol): //End of line get new token
        currentToken = getToken();
        return parse();
    case (token_conditional): //If or while parse it
        int type;

        /**
         * Checks the type of conditional, if it is not if or while returns syntax error
         * */
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
            return NULL;
        }

        currentToken = getToken(); //gets new token after if or while

        //Checks if the token is ( otherwise throws syntax error
        if (currentToken.value == "(")
        {
            //Parses expression inside parantheses
            ASTNode *condition = parseParanExpr();

            //Creates ASTNode to store conditional and expressions
            ConditionalNode *cond = new ConditionalNode(type, condition);

            //If the current token is \n get new token
            while (currentToken.type == token_eol)
            {
                currentToken = getToken();
            }

            //checks "{" otherwise throws syntax error
            if (currentToken.value == "{")
            {
                currentToken = getToken(); //gets next token

                //Gets tokens until it is not end of line
                while (currentToken.type == token_eol)
                {
                    currentToken = getToken();
                }
                //Parse expressions till there is a '}' symbol found or there is no error found
                while (currentToken.value != "}" && !this->error)
                {
                    ASTNode *temp = parseStatement(); //Parses statements inside {} block and adds them to ASTNode

                    if (temp != NULL) //If condition successfully parsed add it to the statement list
                    {
                        cond->statements.push_back(temp);
                    }

                    //There is no '}' file ended. syntax error
                    if (currentToken.type == token_eof)
                    {
                        syntaxError(line);
                        cond = NULL;
                        return cond;
                    }
                }
                currentToken = getToken();
                return cond;
            }
        }
        syntaxError(line);
        return NULL;
    default: //parses statements
        return parseStatement();
    }
}
