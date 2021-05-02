#include "Tokenizer.hpp"

Tokenizer::Tokenizer(ifstream *inputFile)
{
    this->inputFile = inputFile;
    this->lastChar = ' ';
    this->line = 0;
}

Token Tokenizer::getNextToken()
{
    Token tok;

    cout << "tokenizer: " << lastChar << "\n";

    if (!inputFile->eof())
    {
        while (lastChar == '\n' && !inputFile->eof())
        {
            cout << "eol\n";
            tok.value = lastChar;
            tok.type = token_eol;
            tok.line = this->line;
            this->inputFile->get(lastChar);
            this->line++;

            return tok;
        }

        //Removes spaces
        while (lastChar == ' ')
        {
            if (lastChar == '\n'){
                this->line++;
            }

            cout << "space\n"; 
            this->inputFile->get(lastChar);
            if (inputFile->eof())
            {
                tok.type = token_eof;
                tok.value = "_";
                tok.line = this->line;
                return tok;
            }
        }

        //Checks if the first letter is alpha
        if (isalpha(lastChar))
        {

            string identifier = "";
            identifier.push_back(lastChar);

            this->inputFile->get(lastChar);

            while (isalnum(lastChar) && !inputFile->eof())
            {
                identifier.push_back(lastChar);
                this->inputFile->get(lastChar);
            }

            if (identifier == "if" || identifier == "while")
            {
                tok.value = identifier;
                tok.type = token_conditional;
            }
            else if (identifier == "choose")
            {
                tok.value = identifier;
                tok.type = token_choose;
            }
            else if (identifier == "print")
            {
                tok.value = identifier;
                tok.type = token_print;
            }
            else
            {

                tok.value = identifier;
                tok.type = token_identifier;
            }

            tok.line = this->line;
            return tok;
        }

        //checks if it is an integer
        if (isdigit(lastChar))
        {
            string number = "";
            number.push_back(lastChar);
            this->inputFile->get(lastChar);

            while (isdigit(lastChar) && !inputFile->eof())
            {
                number.push_back(lastChar);
                this->inputFile->get(lastChar);
            }

            tok.value = number;
            tok.type = token_number;
            tok.line = this->line;
            return tok;
        }

        //Pass comments
        if (lastChar == '#')
        {

            while (lastChar != '\n')
            {
                cout << "last char: " << lastChar << "\n";

                this->inputFile->get(lastChar);
                if (inputFile->eof())
                {
                    tok.type = token_eof;
                    tok.value = "_";
                    inputFile->get(lastChar);
                    return tok;
                }
            }
            cout << "last char: " << lastChar << "\n";
            return getNextToken();
        }

        tok.value.push_back(lastChar);
        tok.type = token_operator;
        tok.line = this->line;

        inputFile->get(lastChar);

        return tok;
    }

    tok.type = token_eof;
    tok.value = "_";
    inputFile->get(lastChar);
    return tok;
}