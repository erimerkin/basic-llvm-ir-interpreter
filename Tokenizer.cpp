#include "Tokenizer.hpp"


/**
 * Constructor for Tokenizer class
 * */
Tokenizer::Tokenizer(ifstream *inputFile)
{
    this->inputFile = inputFile;
    this->lastChar = ' ';
    this->line = 0;
}

/**
 * Creates tokens by reading input line by line. 
 * Returns a pointer to Token object 
 * */
Token Tokenizer::getNextToken()
{
    Token tok;

    //Checks if it is the end of file 
    if (!inputFile->eof())
    {
        //while it is empty line or end of line increases line count by 1 and returns eol token
        while (lastChar == '\n' && !inputFile->eof())
        {
            //tokenize end of line
            tok.value = lastChar;
            tok.type = token_eol;
            tok.line = this->line;

            this->inputFile->get(lastChar); //get next char
            this->line++; //increase line index

            return tok;
        }

        //Removes spaces
        while (isspace(lastChar))
        {
            //if the type is end of line increase line index
            if (lastChar == '\n')
                this->line++;

            this->inputFile->get(lastChar);
            
            //checks if end of file and returns end of file token
            if (inputFile->eof())
            {
                //tokenize end of file
                tok.type = token_eof;
                tok.value = "_";
                tok.line = this->line;
                return tok;
            }
        }

        //Checks if the first letter is alphabetic
        if (isalpha(lastChar))
        {

            string identifier = "";
            identifier.push_back(lastChar); //add char to string

            this->inputFile->get(lastChar); //gets new char

            //gets new chars until it is not alphanumeric
            while (isalnum(lastChar) && !inputFile->eof())
            {
                identifier.push_back(lastChar);
                this->inputFile->get(lastChar);
            }

            //tokenizes the inputs 
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
            number.push_back(lastChar); //adds char to the string

            this->inputFile->get(lastChar); //gets next char

            //gets char and adds it to string until it is not a digit
            while (isdigit(lastChar) && !inputFile->eof())
            {
                number.push_back(lastChar);
                this->inputFile->get(lastChar);
            }

            //tokenize integer
            tok.value = number;
            tok.type = token_number;
            tok.line = this->line;
            return tok;
        }

        //Pass comments
        if (lastChar == '#')
        {
            //pass until it is end of line
            while (lastChar != '\n')
            {
                this->inputFile->get(lastChar);

                //check end of file and return eof token
                if (inputFile->eof())
                {
                    //returns eof token
                    tok.type = token_eof;
                    tok.value = "_";
                    inputFile->get(lastChar);
                    return tok;
                }
            }
            return getNextToken(); //if it is eol return this function
        }

        //if it is not an identifier, number or special declaration it is a operator.
        //tokenize operator and return
        tok.value.push_back(lastChar);
        tok.type = token_operator;
        tok.line = this->line;

        inputFile->get(lastChar); //get next char

        return tok;
    }

    //End of file, return end of file token
    tok.type = token_eof;
    tok.value = "_";
    inputFile->get(lastChar);
    return tok;
}