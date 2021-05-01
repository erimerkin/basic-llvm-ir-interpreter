#include <string>
#include <vector>
#include <fstream>

using namespace std;

class ASTNode
{
public:
    static int tempIndex;
    string generateCode(ofstream &output);
};

class ChooseNode : public ASTNode
{
public:
    ASTNode *expr1, *expr2, *expr3, *expr4;
    static int chooseIndex;

    ChooseNode(ASTNode *_expr1, ASTNode *_expr2, ASTNode *_expr3, ASTNode *_expr4)
    {
        this->expr1 = _expr1;
        this->expr2 = _expr2;
        this->expr3 = _expr3;
        this->expr4 = _expr4;
    }

    string generateCode(ofstream &output)
    {
        string labelName = "choose_" + chooseIndex;

        string id1 = this->expr1->generateCode(output);
        string id2 = this->expr2->generateCode(output);
        string id3 = this->expr3->generateCode(output);
        string id4 = this->expr4->generateCode(output);

        output << "\tbr label %" << labelName << "if\n\n";

        output << labelName + "if:\n";

        output << "br i1 ";

        labelName + "else";
        labelName + "elif";
        labelName + "end";
    }
};

class PrintNode : public ASTNode
{
public:
    ASTNode *expr;

    PrintNode(ASTNode *_expr)
    {
        this->expr = _expr;
    }

    void generateCode(ofstream &output)
    {
        string text = expr->generateCode(output);
        output << "\tcall i32 (i8*, ...)* @printf(i8* getelementptr ([4 x i8]* @print.str, i32 0, i32 0), i32 %" << text << ")\n";
    }
};

class ConditionalNode : public ASTNode
{
public:
    static int conditionalIndex;
    int type;
    ASTNode *condition;
    vector<ASTNode *> statements;

    ConditionalNode(int _type, ASTNode *_condition)
    {
        this->type = _type;
        this->condition = _condition;
    }

    void generateCode(ofstream &output)
    {
        string conditionName = "cond_" + conditionalIndex;

        output << "\tbr label %" << conditionName <<"entry\n\n";

        output << conditionName << "entry:\n";
        string id = condition->generateCode(output);
        string tempVar = "%temp_var" + tempIndex;
        
        output << "\t" << tempVar << " = icmp ne i32 " << id << ", 0\n";
        output << "\tbr i1 " << tempVar << ", label %" << conditionName << "body, label %" << conditionName << "end\n\n";

        output << conditionName << "body:\n";
        for (auto expression : statements)
        {
            expression->generateCode(output);
        }

        if (this->type == 0)
        {
            output << "\tbr label %" << conditionName << "end\n\n";
        }
        else
        {
            output << "\tbr label %" << conditionName << "entry\n\n";
        }

        output << conditionName << "end:\n";
        tempIndex++;
    }
};

class BinaryOperationNode : public ASTNode
{
public:
    ASTNode *left;
    ASTNode *right;
    char operation;

    BinaryOperationNode(ASTNode *_left, ASTNode *_right, char _operation)
    {
        this->left = _left;
        this->right = _right;
        this->operation = _operation;
    }

    string generateCode(ofstream output)
    {
        string tempId, operand1, operand2, opType;
        operand1 = left->generateCode(output);
        operand2 = right->generateCode(output);
        tempId = "%temp_var" + tempIndex;

        switch (operation)
        {
        case ('+'):
            opType = "add";
        case ('-'):
            opType = "sub";
        case ('*'):
            opType = "mul";
        case ('/'):
            opType = "sdiv";
        }

        output << "\t" << tempId << " = " << opType << " i32 " << operand1 << " , " << operand2 << "\n";

        tempIndex++;
        return tempId;
    }
};

class AssignNode : public ASTNode
{
public:
    IdentifierNode *identifier;
    ASTNode *expr;

    string generateCode(ofstream &output)
    {
        string value = expr->generateCode(output);
        string id = identifier->getID();
        output << "\tstore i32 " << value << ", i32* %" << id << "\n";
    }
};

class NumberNode : public ASTNode
{
public:
    string value;

    NumberNode(string _value)
    {
        this->value = _value;
    }

    string generateCode(ofstream &output)
    {
        return this->value;
    }
};

class IdentifierNode : public ASTNode
{
public:
    string name;

    IdentifierNode(string _name)
    {
        this->name = _name;
    }

    string generateCode(ofstream &output)
    {
        string id = "%temp_var" + tempIndex;
        output << "\t" << id << " = load i32* %" << name << "\n";

        tempIndex++;
        return id;
    }

    string getID(){
        return this->name;
    }
};