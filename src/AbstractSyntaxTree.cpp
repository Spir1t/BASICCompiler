#include <stdexcept>
#include <vector>
#include <string>
#include <memory>
#include "../include/AbstractSyntaxTree.h"

static bool IsIntNode(const std::unique_ptr<ASTNode>& Node)
{
    ASTNode * ptr = Node.get();
    if(dynamic_cast<IntNode *>(ptr)) return true;
    return false;
}

static bool IsFloatNode(const std::unique_ptr<ASTNode>& Node)
{
    ASTNode * ptr = Node.get();
    if(dynamic_cast<FloatNode *>(ptr)) return true;
    return false;
}

static bool IsStringNode(const std::unique_ptr<ASTNode>& Node)
{
    ASTNode * ptr = Node.get();
    if(dynamic_cast<StringNode *>(ptr)) return true;
    return false;
}

static bool IsVariableNode(const std::unique_ptr<ASTNode>& Node)
{
    ASTNode * ptr = Node.get();
    if(dynamic_cast<VariableNode*>(ptr)) return true;
    return false;
}

static bool IsArithmeticOpNode(const std::unique_ptr<ASTNode>& Node)
{
    ASTNode * ptr = Node.get();
    if(dynamic_cast<ArithmeticOpNode*>(ptr)) return true;
    return false;
}

static CType GetCType(const std::unique_ptr<ASTNode>& Node)
{

    if(IsVariableNode(Node))
    {
        VariableNode * ptr = dynamic_cast<VariableNode *>(Node.get());
        return CType::FLOAT;
    }
    else if(IsIntNode(Node))
    {
        IntNode * ptr = dynamic_cast<IntNode *>(Node.get());
        return CType::INT;
    }
    else if(IsFloatNode(Node))
    {
        FloatNode * ptr = dynamic_cast<FloatNode *>(Node.get());
        return CType::FLOAT;
    }
    else if(IsStringNode(Node))
    {
        FloatNode * ptr = dynamic_cast<FloatNode *>(Node.get());
        return CType::STRING;
    }
    else if(IsArithmeticOpNode(Node))
    {
        ArithmeticOpNode * ptr = dynamic_cast<ArithmeticOpNode *>(Node.get());
        for(auto& Operand : ptr->Operands)
        {
            if(IsFloatNode(Operand) || IsVariableNode(Operand)) return CType::FLOAT;
        }
        return CType::INT;
    }
    else
    {
        throw std::runtime_error("AbstractSyntaxTree.cpp - GetCType(): expected variable, int, float or string");
    }
}

static std::string GetCSpecifier(const std::unique_ptr<ASTNode>& Node)
{
    switch(GetCType(Node))
        {
            case CType::INT:
                return("%i");
            case CType::FLOAT:
                return("%f");
            case CType::STRING:
                return("%s");
        }
    return "";
}

std::string ProgramNode::ConvertToC()
{
    std::string Out;
    Out += "#include <stdio.h>\nint main()\n{\n";
    for(auto& Block : Blocks)
    {
        Out += '\t' + Block->ConvertToC() + '\n';
    }
    Out += "\treturn 0;\n}";
    return Out;
}

std::string LineNode::ConvertToC()
{
    return(Content->ConvertToC() + ";");
}

std::string ExpressionNode::ConvertToC()
{
    return(Variable->ConvertToC() + " = " + RightValue->ConvertToC());
}

std::string StringNode::ConvertToC()
{
    return(Value);
}

std::string IntNode::ConvertToC()
{
    return(Value);
}

std::string FloatNode::ConvertToC()
{
    return(Value);
}

std::string VariableNode::ConvertToC()
{
    return(Name);
}

std::string ArithmeticOpNode::ConvertToC()
{
    std::string Out;
    int i = 0;
    while(i < Operations.size())
    {
        Out += Operands[i]->ConvertToC() + ' ' + Operations[i] + ' ';
        i++;
    }
    Out += Operands[i]->ConvertToC();
    return Out;
}

std::string PrintNode::ConvertToC()
{
    std::string Out("printf(\"");
    std::vector<std::string> Strs;
    std::vector<std::string> Args;
    for(auto& Node : Contents)
    {
        if(IsStringNode(Node))
        {
            Strs.push_back(Node->ConvertToC());
        }
        else
        {
            Strs.push_back(GetCSpecifier(Node));
            Args.push_back(Node->ConvertToC());
        }
    }
    for(auto str : Strs) {Out += str;}
    Out += "\\n\"";
    for(auto arg : Args) {Out += ", " + arg;}
    Out += ')';
    return Out;
}

std::string StopNode::ConvertToC()
{
    return("exit(0)");
}

std::string LetNode::ConvertToC()
{
    return("float " + Expression->ConvertToC());
}

std::string IfCondNode::ConvertToC()
{
    return(If->ConvertToC() + '\n' + Then->ConvertToC());
}

std::string IfElseCondNode::ConvertToC()
{
    return(If->ConvertToC() + '\n' + Then->ConvertToC() + '\n' + Else->ConvertToC());
}

std::string IfNode::ConvertToC()
{
    return("if(" + Condition->ConvertToC() + ")");
}

std::string ThenNode::ConvertToC()
{
    if(dynamic_cast<IfCondNode *>(Block.get()) || dynamic_cast<IfElseCondNode *>(Block.get()) 
    || dynamic_cast<WhileNode *>(Block.get()))
    {
        return("\t{\n\t" + Block->ConvertToC() + "\n\t}");
    }
    return("\t{\n\t\t" + Block->ConvertToC() + "\n\t}");
}

std::string ElseNode::ConvertToC()
{
    if(dynamic_cast<IfCondNode *>(Block.get()) || dynamic_cast<IfElseCondNode *>(Block.get()) 
    || dynamic_cast<WhileNode *>(Block.get()))
    {
        return("\telse\n\t{\n\t" + Block->ConvertToC() +"\n\t}");
    }
    return("\telse\n\t{\n\t\t" + Block->ConvertToC() + "\n\t}");
}

std::string ConditionNode::ConvertToC()
{
    std::string Out = Statements[0]->ConvertToC();
    int i = 0;
    while(i < LogicOps.size())
    {
        Out += + " " + LogicOps[i] + " " + Statements[i+1]->ConvertToC();
        i++;
    }
    return Out;
}

std::string StatementNode::ConvertToC()
{
    return(Not + LeftSide->ConvertToC() + " " + Comparison + " " + RightSide->ConvertToC());
} 
std::string WhileNode::ConvertToC()
{
    std::string out = "while(" + Condition->ConvertToC() + ")\n\t{\n";
    for(auto& Body : WhileBodies)
    {
        out += "\t\t" + Body->ConvertToC() + "\n";
    }
    out += "\t}";
    return out;
}