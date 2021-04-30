#include "SyntaxTree.cpp"

ExpressionNode *parseParanExpr()
{
    getNextToken();

    if (CurTok != ')')
        return syntaxError(line);
    getNextToken(); // eat ).
    return V;
}

ExpressionNode *parseFactor()
{
}

ExpressionNode *parseMorefactor()
{
}

ExpressionNode *parseTerm()
{
}

ExpressionNode *parseChoose()
{
}

ExpressionNode *parsePrint()
{
}

ExpressionNode *parseMoreterms()
{
}

ExpressionNode *parseExpr()
{
}
