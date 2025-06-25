#pragma once
#include <string>
#include <vector>
#include <memory>

enum class CType
{
    INT = 0,
    FLOAT,
    STRING,
};

struct ASTNode 
{
    virtual ~ASTNode() = default;
    virtual std::string ConvertToC() = 0;
};

struct ProgramNode : ASTNode
{
    //ExpressionNode, BasicFunctionNode, CondOpNode, WhileNode
    std::vector<std::unique_ptr<ASTNode>> Blocks;
    virtual std::string ConvertToC() override;
};

struct LineNode : ASTNode
{
    //ExpressionNode OR PrintNode OR StopNode OR LetNode
    std::unique_ptr<ASTNode> Content;
    virtual std::string ConvertToC() override;
};

struct ExpressionNode : ASTNode
{
    //VariableNode
    std::unique_ptr<ASTNode> Variable;
    //StringNode, IntNode, FloatNode, VariableNode, BinOpNode
    std::unique_ptr<ASTNode> RightValue;
    virtual std::string ConvertToC() override;
};

struct IntNode : ASTNode
{
    std::string Value;
    std::string Shift;
    virtual std::string ConvertToC() override;
};

struct FloatNode : ASTNode
{
    std::string Value;
    virtual std::string ConvertToC() override;
};

struct StringNode : ASTNode
{
    std::string Value;
    virtual std::string ConvertToC() override;
};

struct VariableNode : ASTNode
{
    std::string Name;
    virtual std::string ConvertToC() override;
};

struct ArithmeticOpNode : ASTNode
{
    //intNode, floatNode, SymbolsNode
    std::vector<std::unique_ptr<ASTNode>> Operands;
    std::vector<char> Operations;

    virtual std::string ConvertToC() override;
};

struct PrintNode : ASTNode
{
    //IntNode, FloatNode, StringNode, VariableNode, ArithmeticOpNode
    std::vector<std::unique_ptr<ASTNode>> Contents; 
    virtual std::string ConvertToC() override;
};

struct StopNode : ASTNode
{
    virtual std::string ConvertToC() override;
};

struct LetNode : ASTNode
{
    std::unique_ptr<ASTNode> Expression;
    virtual std::string ConvertToC() override;
};

struct IfCondNode : ASTNode
{
    //IfNode
    std::unique_ptr<ASTNode> If;
    //ThenNode
    std::unique_ptr<ASTNode> Then;
    virtual std::string ConvertToC() override;
};

struct IfElseCondNode : ASTNode
{
    //IfNode
    std::unique_ptr<ASTNode> If;
    //ThenNode
    std::unique_ptr<ASTNode> Then;
    //ElseNode
    std::unique_ptr<ASTNode> Else;
    virtual std::string ConvertToC() override;
};

struct IfNode : ASTNode
{
    //ConditionNode
    std::unique_ptr<ASTNode> Condition;
    virtual std::string ConvertToC() override;

    IfNode(std::unique_ptr<ASTNode> Cond) : Condition(std::move(Cond)) {}
    IfNode() = default;
};

struct ThenNode : ASTNode
{
    //ExpressionNode OR PrintNode OR StopNode OR LetNode
    std::unique_ptr<ASTNode> Block;
    virtual std::string ConvertToC() override;

    ThenNode(std::unique_ptr<ASTNode> Bl) : Block(std::move(Bl)) {}
    ThenNode() = default;
};

struct ElseNode : ASTNode
{
    //ExpressionNode OR PrintNode OR StopNode OR LetNode
    std::unique_ptr<ASTNode> Block;
    virtual std::string ConvertToC() override;
};

struct ConditionNode : ASTNode
{
    //StatementNode
    std::vector<std::unique_ptr<ASTNode>> Statements;
    std::vector<std::string> LogicOps;
    virtual std::string ConvertToC() override;
};

struct StatementNode : ASTNode
{
    std::string Not;
    //IntNode OR FloatNode OR VariableNode
    std::unique_ptr<ASTNode> LeftSide;
    std::string Comparison;
    //IntNode OR FloatNode OR VariableNode
    std::unique_ptr<ASTNode> RightSide;
    virtual std::string ConvertToC() override;
};

struct WhileNode : ASTNode
{
    //ConditionNode
    std::unique_ptr<ASTNode> Condition;
    //WhileBodyNode
    std::vector<std::unique_ptr<ASTNode>> WhileBodies;
    virtual std::string ConvertToC() override;
};