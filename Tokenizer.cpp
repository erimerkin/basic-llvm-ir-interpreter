#include "Tokenizer.h"

Tokenizer::Tokenizer(ifstream *inputFile)
{
    this->inputFile = inputFile;
}

Token *Tokenizer::getNextToken()
{
    Token *tok;

    char lastChar;

    this->inputFile->get(lastChar);

    while (!this->inputFile->eof())
    {
        //Removes spaces
        while (isspace(lastChar))
        {
            this->inputFile->get(lastChar);
        }

        //Checks if the first letter is alpha
        if (isalpha(lastChar))
        {
            string identifier = "" + lastChar;

            this->inputFile->get(lastChar);
            while (isalnum(lastChar))
            {
                identifier += lastChar;
                this->inputFile->get(lastChar);
            }

            if (identifier == "if" || identifier == "while")
            {
                tok->value = identifier;
                tok->type = token_conditional;
            }
            else if (identifier == "choose")
            {
                tok->value = identifier;
                tok->type = token_choose;
            }
            else if (identifier == "print")
            {
                tok->value = identifier;
                tok->type = token_print;
            }
            else
            {
                tok->value = identifier;
                tok->type = token_identifier;
            }

            return tok;
        }

        //checks if it is an integer
        if (isdigit(lastChar))
        {
            string number = "" + lastChar;
            this->inputFile->get(lastChar);

            while (isdigit(lastChar))
            {
                number += lastChar;
                this->inputFile->get(lastChar);
            }

            tok->value = number;
            tok->type = token_number;

            return tok;
        }

        //Pass comments
        if (lastChar == '#')
        {
            do
                this->inputFile->get(lastChar);
            while (lastChar != EOF && lastChar != '\n' && lastChar != '\r');
        }

        if (lastChar == EOF)
        {
            tok->type = token_eof;
            return tok;
        }

        if (lastChar == '\n')
        {
            tok->value = lastChar;
            tok->type = token_eol;
            return tok;
        }

        tok->value = lastChar;
        tok->type = token_operator;
        return tok;
    }

    tok->type = token_eof;
    return tok;
}