#include <fstream>
#include <string>

#include "Parser.hpp"

using namespace std;

/**
 * Creates the code for syntax error in given line
 * */
void syntaxError(int line, ofstream &output)
{
    output << "; ModuleID = \'mylang2ir\'\n"
           << "declare i32 @printf(i8*, ...)\n"
           << "@print.str = constant [23 x i8] c\"Line %d: syntax error\\0A\\00\"\n\n"
           << "define i32 @main() {\n"
           << "\tcall i32 (i8*, ...)* @printf(i8* getelementptr ([23 x i8]* @print.str, i32 0, i32 0), i32 " << line << ")\n"
           << "\tret i32 0\n"
           << "}";
}

/**
 * Generates IR code by adding headers to file, allocating space for all variables 
 * and running generateCode() function from AST nodes which creates IR code for its type.
 * Takes parameters ofstream output to print to file, vector<ASTNode> program is the AST
 * and varmap stores all the declared variables which is used to allocate them.
 * */
void generateIR(ofstream &output, vector<ASTNode *> &program, unordered_set<string> &varmap)
{

    //Adding header to .ll file
    output << "; ModuleID = \'mylang2ir\'\n"
           << "declare i32 @printf(i8*, ...)\n"
           << "@print.str = constant [4 x i8] c\"%d\\0A\\00\"\n\n"
           << "define i32 @main() {\n";

    //Allocates declared variables
    for (auto test : varmap)
    {
        output << "\t%" << test << " = alloca i32\n";
    }

    output << "\n";

    //Gives all allocated vars value of 0
    for (auto identifier : varmap)
    {
        output << "\tstore i32 0, i32* %" << identifier << "\n";
    }

    output << "\n";

    //Creates the code from given program
    for (auto expression : program)
    {
        expression->generateCode(output);
    }

    //Finishes the code creation
    output << "\tret i32 0\n"
           << "}";
}

int main(int argc, char *argv[])
{

    vector<ASTNode *> program; //vector to hold Nodes for code generation

    string inputFile = argv[1]; 
    string outputFile = inputFile.substr(0, inputFile.size() - 3) + ".ll";

    ifstream inFile(inputFile);

    Tokenizer *tokenizer = new Tokenizer(&inFile);
    Parser *parser = new Parser(tokenizer);

    //Parse till there is an error or it is end of file
    while (!parser->error && parser->currentToken.type != token_eof)
    {

        ASTNode *node = parser->parse(); //parses file

        if (node != NULL)
            program.push_back(node);
        else
            break;
    }

    inFile.close();

    ofstream outFile(outputFile);

    //if there is an error generate error code
    if (parser->error)
    {
        syntaxError(parser->errLine, outFile);
        return 0;
    }

    //generate code
    generateIR(outFile, program, parser->variables);

    outFile.close();

    return 0;
}