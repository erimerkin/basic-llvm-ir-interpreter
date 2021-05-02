#ifndef ASTNODE_HPP
#define ASTNODE_HPP

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

class ASTNode
{
public:
    static int tempIndex;
    virtual string generateCode(ofstream &output) = 0;
};

class IdentifierNode : public ASTNode
{
public:
    string name;
    IdentifierNode(string _name);
    string generateCode(ofstream &output);
    string getID();
};

class NumberNode : public ASTNode
{
public:
    string value;
    NumberNode(string _value);
    string generateCode(ofstream &output);
};

class ChooseNode : public ASTNode
{
public:
    ASTNode *expr1, *expr2, *expr3, *expr4;
    static int chooseIndex;
    ChooseNode(ASTNode *_expr1, ASTNode *_expr2, ASTNode *_expr3, ASTNode *_expr4);
    string generateCode(ofstream &output);
};

class BinaryOperationNode : public ASTNode
{
public:
    ASTNode *left;
    ASTNode *right;
    char operation;

    BinaryOperationNode(ASTNode *_left, ASTNode *_right, char _operation);
    string generateCode(ofstream &output);
};

class PrintNode : public ASTNode
{
public:
    ASTNode *expr;
    PrintNode(ASTNode *_expr);
    string generateCode(ofstream &output);
};

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

class AssignNode : public ASTNode
{
public:
    IdentifierNode *identifier;
    ASTNode *expr;
    AssignNode(IdentifierNode *id, ASTNode *expr);
    string generateCode(ofstream &output);
};

#endif