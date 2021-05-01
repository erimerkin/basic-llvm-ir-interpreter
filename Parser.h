#include "SyntaxTree.cpp"

#include "Tokenizer.h"

using namespace std;
class Parser
{
public:

    Tokenizer* tokenizer;
    static int index;
    int line;
    vector<string> variables;
    Token currentToken, lastToken;

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