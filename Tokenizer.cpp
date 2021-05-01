#include "Tokenizer.h"

void getToken()
{
    //Removes spaces
    while (isspace(lastChar))
    {
        lastChar = getNextChar();
    }

    //Checks if the first letter
    if (isalpha(lastChar))
    {
        string identifier = lastChar;

        lastChar = getNextChar();
        while (isalnum(lastChar))
        {
            identifier += lastChar;
            lastChar = getNextChar();
        }
    }

    //checks if the next
    if (isdigit(lastChar))
    {
        string number = lastChar;
        lastChar = getNextChar();

        while (isdigit(lastChar))
        {
        }
    }

    if (lastChar == #)
    {
        continue;
    }
}