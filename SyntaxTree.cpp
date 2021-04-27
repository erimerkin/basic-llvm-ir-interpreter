class ExpressionNode
{
};

class ChooseNode : public ExpressionNode
{
public:
    ExpressionNode *expr1, *expr2, *expr3, *expr4;
};

class PrintNode : public ExpressionNode
{
    public:
    
};

class ConditionalNode : public ExpressionNode {
    public:
    string type;
    ExpressionNode *condition;
    ExpressionNode[] *statements;
};

class BinaryOperationNode : public ExpressionNode
{
public:
    ExpressionNode *left;
    ExpressionNode *right;
    std::string operation;

};

class NumberNode : public ExpressionNode
{
    public:
    int value;
};

class IdentifierNode : public ExpressionNode
{
public:
    string name;
    int value;
};