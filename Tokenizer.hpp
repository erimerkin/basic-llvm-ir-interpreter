#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

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
    string value;
    TokenType type;
    int line;
};

class Tokenizer
{
public:
    ifstream *inputFile;
    char lastChar;
    int line;

    Tokenizer(ifstream *inputFile);
    ~Tokenizer();

    // void tokenizeInput(ifstream &input);
    Token getNextToken();
};

#endif