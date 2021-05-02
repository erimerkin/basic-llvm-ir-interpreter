#ifndef ASTNODE_HPP
#define ASTNODE_HPP

#include <string>
#include <vector>
#include <fstream>

using namespace std;

/**
 * Abstract class for Asynchronous Syntax Tree
 * */
class ASTNode
{
public:
    static int tempIndex;
    virtual string generateCode(ofstream &output) = 0;
 };

/**
 * Stores identifier of variables. Can generate code with temp variables
 * */
class IdentifierNode : public ASTNode
{
public:
    string name;
    IdentifierNode(string _name);
    string generateCode(ofstream &output);
    string getID();
};


/**
 * Stores values for numbers. generateCode() function returns the integer value
 * */
class NumberNode : public ASTNode
{
public:
    string value;
    NumberNode(string _value);
    string generateCode(ofstream &output);
};

/**
 * Node for choose expression, stores the expressions inside parantheses. 
 * Generates code for both expressions and choose function.
 * */
class ChooseNode : public ASTNode
{
public:
    ASTNode *expr1, *expr2, *expr3, *expr4;
    static int chooseIndex;
    ChooseNode(ASTNode *_expr1, ASTNode *_expr2, ASTNode *_expr3, ASTNode *_expr4);
    string generateCode(ofstream &output);
};

/**
 * Node for binary operations. Stores the operation type, right and left handside as expressions
 * and generates code for the calculation
 * */
class BinaryOperationNode : public ASTNode
{
public:
    ASTNode *left;
    ASTNode *right;
    char operation;

    BinaryOperationNode(ASTNode *_left, ASTNode *_right, char _operation);
    string generateCode(ofstream &output);
};

/**
 * Node for print statements. Generates code for print statement and expression inside the statement
 * */
class PrintNode : public ASTNode
{
public:
    ASTNode *expr;
    PrintNode(ASTNode *_expr);
    string generateCode(ofstream &output);
};

/**
 * Node to store conditional statements. Stores the condition, conditional type and statements
 * inside the code block and generates code for all of them
 * */
class ConditionalNode : public ASTNode
{
public:
    static int conditionalIndex;
    int type;
    ASTNode *condition;
    vector<ASTNode *> statements;

    ConditionalNode(int _type, ASTNode *_condition);
    string generateCode(ofstream &output);
};

/**
 * Node for assignment statement. Stores identifier and expression 
 * Generates code for assignment statement
 * */
class AssignNode : public ASTNode
{
public:
    IdentifierNode *identifier;
    ASTNode *expr;
    AssignNode(IdentifierNode *id, ASTNode *expr);
    string generateCode(ofstream &output);
};

#endif