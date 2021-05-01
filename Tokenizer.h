#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>
#include <fstream>

using namespace std;

enum TokenType
{
    token_identifier,
    token_number,
    token_operator,
    token_print,
    token_choose,
    token_conditional,
    token_eol,
    token_eof
};

struct Token
{
    int line;
    string value;
    TokenType type;
};

class Tokenizer
{
public:
    static int lineNumber, lastLine;
    // vector<Token> tokens;
    ifstream *inputFile;

    Tokenizer(ifstream *inputFile);
    ~Tokenizer();

    // void tokenizeInput(ifstream &input);
    Token* getNextToken();
};

#endif