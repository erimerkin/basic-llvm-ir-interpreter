#include "ASTNode.hpp"

/***********
 * IdentifierNode
 *************/

IdentifierNode::IdentifierNode(string _name)
{
    this->name = _name;
}

/**
 * Loads given variable to a temp variable
 * */
string IdentifierNode::generateCode(ofstream &output)
{
    string id = "%temp_var" + to_string(tempIndex);
    tempIndex++;

    output << "\t" << id << " = load i32* %" << name << "\n";

    return id;
}

/**
 * Returns the identifier of the variable for assignment statements
 * */
string IdentifierNode::getID()
{
    return this->name;
}

/****************
 * NumberNode
 * **************/

NumberNode::NumberNode(string _value)
{
    this->value = _value;
}

/**
 * Returns the value of number for expression code generation
 * */
string NumberNode::generateCode(ofstream &output)
{
    return this->value;
}

/****************
 * ChooseNode
 * **************/

ChooseNode::ChooseNode(ASTNode *_expr1, ASTNode *_expr2, ASTNode *_expr3, ASTNode *_expr4)
{
    this->expr1 = _expr1;
    this->expr2 = _expr2;
    this->expr3 = _expr3;
    this->expr4 = _expr4;
}

string ChooseNode::generateCode(ofstream &output)
{
    string labelName = "choose_" + to_string(chooseIndex);
    chooseIndex++;

    string ret_var = "%temp_var" + to_string(tempIndex);
    tempIndex++;
    output << "\t" << ret_var << " = alloca i32\n"
           << "\tstore i32 0, i32* " << ret_var << "\n";

    string labelif = labelName + "if";
    string el = labelName + "else";
    string elif = labelName + "elif";
    string end = labelName + "end";

    string id1 = this->expr1->generateCode(output);
    // string id2 = this->expr2->generateCode(output);
    // string id3 = this->expr3->generateCode(output);
    // string id4 = this->expr4->generateCode(output);

    output << "\tbr label %" << labelif << "\n\n";

    //IF COND
    output << labelif << ":\n";
    string tempVar1 = "%temp_var" + to_string(tempIndex);
    tempIndex++;
    output << "\t" << tempVar1 << " = icmp eq i32 " << id1 << ", 0\n";
    output << "\tbr i1 " << tempVar1 << ", label %" << labelif << "body, label %" << elif << "\n\n";

    //IF BODY
    output << labelif << "body:\n";
    string tempVar2 = "%temp_var" + to_string(tempIndex);
    tempIndex++;

    // output << "\t" << tempVar2 << " = load i32* " << expr2->generateCode(output) << "\n"
    string id2 = this->expr2->generateCode(output);
    output << "\tstore i32 " << id2 << ", i32* " << ret_var << "\n"
           << "\tbr label %" << end << "\n\n";

    //ELSE IF COND
    output << elif << ":\n";
    string tempVar3 = "%temp_var" + to_string(tempIndex);
    tempIndex++;
    output << "\t" << tempVar3 << " = icmp sgt i32 " << id1 << ", 0\n";
    output << "\tbr i1 " << tempVar3 << ",label %" << elif << "body, label %" << el << "\n\n";

    //ELSE IF BODY
    output << elif << "body:\n";
    string tempVar4 = "%temp_var" + to_string(tempIndex);
    tempIndex++;
    // output << "\t" << tempVar4 << " = load i32* " << this->expr3->generateCode(output) << "\n"
    string id3 = this->expr3->generateCode(output);
    output << "\tstore i32 " << id3 << ", i32* " << ret_var << "\n"
           << "\tbr label %" << end << "\n\n";

    //ELSE BODY
    output << el << ":\n";
    string tempVar5 = "%temp_var" + to_string(tempIndex);
    tempIndex++;

    string id4 = this->expr4->generateCode(output);
    // output << "\t" << tempVar5 << " = load i32* " << this->expr4->generateCode(output) << "\n"
    output << "\tstore i32 " << id4 << ", i32* " << ret_var << "\n"
           << "\tbr label %" << end << "\n\n";

    //END
    output << end << ":\n";
    string tempVar6 = "%temp_var" + to_string(tempIndex);
    tempIndex++;
    output << "\t" << tempVar6 << " = load i32* " << ret_var << "\n";

    return tempVar6;
}

/****************
 * BinaryOperationNode
 * **************/

BinaryOperationNode::BinaryOperationNode(ASTNode *_left, ASTNode *_right, char _operation)
{
    this->left = _left;
    this->right = _right;
    this->operation = _operation;
}

/**
 * Generates code for binary operations by calling left and right handside code generation first
 * */
string BinaryOperationNode::generateCode(ofstream &output)
{
    string tempId, operand1, operand2, opType;
    operand1 = left->generateCode(output);  //Generate left side code
    operand2 = right->generateCode(output); //Generate right side code

    tempId = "%temp_var" + to_string(tempIndex);
    tempIndex++;

    //Checks operation type
    switch (operation)
    {
    case ('+'):
        opType = "add";
        break;
    case ('-'):
        opType = "sub";
        break;
    case ('*'):
        opType = "mul";
        break;
    case ('/'):
        opType = "sdiv";
        break;
    }

    output << "\t" << tempId << " = " << opType << " i32 " << operand1 << " , " << operand2 << "\n";

    return tempId;
}

/****************
 * PrintNode
 * **************/

PrintNode::PrintNode(ASTNode *_expr)
{
    this->expr = _expr;
}

/**
*  Generates print code by firstly calling code generation of expression inside print statement
* */
string PrintNode::generateCode(ofstream &output)
{
    string text = expr->generateCode(output);
    output << "\tcall i32 (i8*, ...)* @printf(i8* getelementptr ([4 x i8]* @print.str, i32 0, i32 0), i32 " << text << ")\n";
    return "";
}

/****************
 * ConditionalNode
 * **************/

ConditionalNode::ConditionalNode(int _type, ASTNode *_condition)
{
    this->type = _type;
    this->condition = _condition;
}

/**
 * Generates code for conditional statements, first generates the condition code
 * then generates the code in {} block
 * */
string ConditionalNode::generateCode(ofstream &output)
{

    string conditionName = "cond_" + to_string(conditionalIndex);
    conditionalIndex++;

    output << "\tbr label %" << conditionName << "entry\n\n";

    output << conditionName << "entry:\n";

    string id = condition->generateCode(output); //Generates condition code
    string tempVar = "%temp_var" + to_string(tempIndex);
    tempIndex++;

    output << "\t" << tempVar << " = icmp ne i32 " << id << ", 0\n";
    output << "\tbr i1 " << tempVar << ", label %" << conditionName << "body, label %" << conditionName << "end\n\n";

    output << conditionName << "body:\n";

    //Generates code for the statements inside conditional
    for (auto expression : statements)
    {
        expression->generateCode(output);
    }

    //checks type op conditional and adjusts the code for it
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

    return "";
}

/****************
 * AssignNode
 * **************/

AssignNode::AssignNode(IdentifierNode *id, ASTNode *expr)
{
    this->identifier = id;
    this->expr = expr;
}

/**
 * Code generation for assignment statements
 * */
string AssignNode::generateCode(ofstream &output)
{
    string value = expr->generateCode(output);
    string id = identifier->getID();
    output << "\tstore i32 " << value << ", i32* %" << id << "\n";
    return "";
}
