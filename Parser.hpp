#ifndef PARSER_HPP
#define PARSER_HPP

#include <unordered_set>

#include "ASTNode.hpp"
#include "Tokenizer.hpp"

using namespace std;
class Parser
{
public:
    Tokenizer *tokenizer;
    int line;
    bool error;
    unordered_set<string> variables;
    Token currentToken, lastToken;

    Parser(Tokenizer *_tokenizer);

    ASTNode *parseParanExpr();
    ASTNode *parseFactor();
    ASTNode *parseMorefactor();
    ASTNode *parseTerm();
    ASTNode *parsePrint();
    ASTNode *parseChoose();
    ASTNode *parseMoreterms();
    ASTNode *parseExpr();
    ASTNode *parseStatement();
    ASTNode *parse();

    void syntaxError(int line);
    Token getToken();
};

#endif