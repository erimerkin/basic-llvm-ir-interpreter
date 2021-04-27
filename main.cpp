#include <iostream>
#include <map>
#include <fstream>
#include <string>

using namespace std;

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

void syntaxError(int line)
{
}

char getNextChar()
{
}

int main(int argc, char *argv[])
{

    Map<string, int> varmap;

    string inputFile = argv[1];
    string outputFile = inputFile.substr(0, inputFile.size() - 3);

    ifstream inFile(inputFile);

    int lineIndex;
    string line;
    while(getline(inFile, line){
        lineIndex++;
        static int lastChar = ' ';

    }
}