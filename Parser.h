#ifndef PARSER_H
#define PARSER_H

#include <unordered_set>

#include "SyntaxTree.cpp"
#include "Tokenizer.h"

using namespace std;
class Parser
{
public:
    Tokenizer *tokenizer;
    static int index;
    int line;
    bool error;
    unordered_set<string> variables;
    Token *currentToken, lastToken;

    Parser(Tokenizer &_tokenizer);

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
};

#endif