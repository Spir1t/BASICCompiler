#include <stdexcept>
#include <string>
#include <vector>
#include <memory>
#include <iostream> //убрать
#include "../include/Parser.h"

namespace debug
{
    bool DoDebug = false;
    bool DoExtendedDebug = false;
    void log(std::string msg)
    {
        if(DoDebug) std::cout << msg << std::endl;
    }
    void ExtendedLog(std::string msg)
    {
        if(DoDebug && DoExtendedDebug) std::cout << msg << std::endl;
    }
}

Token Parser::Current()
{
    if(Pos < Tokens.size()) return Tokens[Pos];
    return {TokenType::EndOfFile, "\0"};
}

Token Parser::Next()
{
    if(Pos + 1 < Tokens.size()) return Tokens[Pos + 1];
    return {TokenType::EndOfFile, "\0"};
}

Token Parser::NextAfterNext()
{
    if(Pos + 2 < Tokens.size()) return Tokens[Pos + 2];
    return {TokenType::EndOfFile, "\0"};
}

void Parser::Expect(TokenType Type)
{
    if(!Match(Type))
    {
        throw std::runtime_error("Syntax error: expected " + Type2String(Type) + ", got " + Type2String(CurrentType()));
    }
}

void Parser::Expect(std::vector<TokenType> Types)
{
    for(auto Type : Types)
    {
        if(Match(Type))
        {
            return;
        }
    }
    throw std::runtime_error("Syntax error: got " + Type2String(CurrentType()));
}

Token Parser::Eat(TokenType Type)
{
    debug::ExtendedLog("Eat: " + Current().GetValue());
    Expect(Type);
    Token buf = Current();
    Iterate();
    return buf;
}

std::unique_ptr<ProgramNode> Parser::ParseProgram(std::vector<Token>& NewTokens)
{
    debug::log("ParseProgram");
    Tokens = NewTokens;
    auto program = std::make_unique<ProgramNode>();
    while(!Match(TokenType::EndOfFile))
    {
        program->Blocks.push_back(ParseBlock());
    }
    debug::ExtendedLog("ParseProgram ENDED");
    return program;
}

std::unique_ptr<ASTNode> Parser::ParseBlock()
{
    debug::log("ParseBlock");
    if(Match(TokenType::IF)) {return ParseConditionalOp();}
    else if(Match(TokenType::WHILE)) {return ParseWhile();}
    else {return ParseLine();}
    debug::ExtendedLog("ParseBlock ENDED");
}

std::unique_ptr<ASTNode> Parser::ParseLine()
{
    debug::log("ParseLine");
    auto Line = std::make_unique<LineNode>();
    if(Match(TokenType::VARIABLE)) Line->Content = ParseExpression();
    else Line->Content = ParseBasicFunction();
    return Line;
}

std::unique_ptr<ASTNode> Parser::ParseConditionalOp()
{
    debug::log("ParseConditionalOp");

    auto If = ParseIf();
    auto Then = ParseThen();
    if(Match(TokenType::ELSE))
    {
        debug::log("ParseIfElseCond");
        auto IfElseCond = std::make_unique<IfElseCondNode>();
        IfElseCond->If = std::move(If);
        IfElseCond->Then =std::move(Then);
        IfElseCond->Else = ParseElse();
        debug::ExtendedLog("ParseIfElseCond ENDED");
        return IfElseCond;
    }
    debug::log("ParseIfCond");
    auto IfCond = std::make_unique<IfCondNode>();
    IfCond->If = std::move(If);
    IfCond->Then = std::move(Then);
    debug::ExtendedLog("ParseIfCond ENDED");
    return IfCond;
}


std::unique_ptr<ASTNode> Parser::ParseIf()
{
    debug::log("ParseIf");
    auto If = std::make_unique<IfNode>();
    Eat(TokenType::IF);
    If->Condition = ParseCondition();
    return If;
}

std::unique_ptr<ASTNode> Parser::ParseThen()
{
    debug::log("ParseThen");
    auto Then = std::make_unique<ThenNode>();
    Eat(TokenType::THEN);
    Then->Block = ParseBlock();
    debug::ExtendedLog("ParseThen ENDED");
    return Then;
}

std::unique_ptr<ASTNode> Parser::ParseElse()
{
    debug::log("ParseElse");
    auto Else = std::make_unique<ElseNode>();
    Eat(TokenType::ELSE);
    Else->Block = ParseBlock();
    return Else;
}

std::unique_ptr<ASTNode> Parser::ParseCondition()
{
    debug::log("ParseCondition");
    auto Condition = std::make_unique<ConditionNode>();
    Condition->Statements.push_back(ParseStatement());
    bool CurrentIsLogicOperation = CurrentType() == TokenType::AND || CurrentType() == TokenType::OR;
    bool Check = CurrentIsLogicOperation;
    int count = 0;
    while(Check)
    {
        if(CurrentType() == TokenType::AND) 
        {
            Eat(TokenType::AND);
            Condition->LogicOps.push_back("&&");
        }
        else if(CurrentType() == TokenType::OR)
        {
            Eat(TokenType::OR);
            Condition->LogicOps.push_back("||");
        }
        else
        {
            Condition->Statements.push_back(ParseStatement());
        }
        count++;
        CurrentIsLogicOperation = Match(TokenType::AND) || Match(TokenType::OR);
        Check = CurrentIsLogicOperation && count % 2 == 0 || (!CurrentIsLogicOperation && count % 2 == 1);
    }
    debug::ExtendedLog("ParseCondition ENDED");
    return Condition;
}

std::unique_ptr<ASTNode> Parser::ParseStatement()
{
    debug::log("ParseStatement");
    auto Statement = std::make_unique<StatementNode>();
    if(Match(TokenType::NOT)) 
    {
        Statement->Not = "!";
        Eat(TokenType::NOT);
    }
    Statement->LeftSide = ParseOperand();
    Statement->Comparison = ParseComparison();
    Statement->RightSide = ParseOperand();
    debug::log("ParseStatement ENDED");
    return Statement;
}

std::string Parser::ParseComparison()
{
    debug::log("ParseComparison");
    if(Match(TokenType::EQUAL)) 
    {
        Eat(TokenType::EQUAL);
        return "=";
    }
    else if(Match(TokenType::MORE)) 
    {
        Eat(TokenType::MORE);
        return ">";
    }
    else if(Match(TokenType::LESS)) 
    {
        Eat(TokenType::LESS);
        return "<";
    }
    else if(Match(TokenType::MORE_EQUAL) )
    {
        Eat(TokenType::MORE_EQUAL);
        return ">=";
    }
    else if(Match(TokenType::LESS_EQUAL)) 
    {
        Eat(TokenType::LESS_EQUAL);
        return "<=";
    }
    else 
    {
        Eat(TokenType::NOT_EQUAL);
        return "<>";
    }
}

std::unique_ptr<ASTNode> Parser::ParseWhile()
{
    auto While = std::make_unique<WhileNode>();
    Eat(TokenType::WHILE);
    While->Condition = ParseCondition();
    Eat(TokenType::REPEAT);
    while(!Match(TokenType::ENDWHILE))
    {
        While->WhileBodies.push_back(ParseBlock());
    }
    Eat(TokenType::ENDWHILE);
    return While;
}

std::unique_ptr<ASTNode> Parser::ParseExpression()
{
    debug::log("ParseExpression");
    auto Expression = std::make_unique<ExpressionNode>();
    Expression->Variable = ParseVariable();
    Expect(TokenType::EQUAL);
    Iterate();
    if(Next().GetType() == TokenType::ADD || Next().GetType() == TokenType::SUBSTRACT 
    || Next().GetType() == TokenType::MULTIPLY || Next().GetType() == TokenType::DIVIDE) 
    {
        Expression->RightValue = ParseArithmeticOp();
    }
    else
    {
        Expression->RightValue = ParseOperand();
    }
    return Expression;
}

std::unique_ptr<ASTNode> Parser::ParseOperand()
{
    debug::log("ParseOperand");
    if(Match(TokenType::INT)) return ParseInt();
    else if(Match(TokenType::FLOAT)) return ParseFloat();
    else return ParseVariable();
}

std::unique_ptr<ASTNode> Parser::ParseBasicFunction()
{
    debug::log("ParseBasicFunction");
    if(Match(TokenType::PRINT)) {return ParsePrint();}
    else if(Match(TokenType::STOP)) {return ParseStop();}
    else {return ParseLet();}
}

std::unique_ptr<ASTNode> Parser::ParsePrint()
{
    debug::log("ParsePrint");
    auto Print = std::make_unique<PrintNode>();
    Eat(TokenType::PRINT);
    bool ExpectInt = Match(TokenType::INT);
    bool ExpectFloat = Match(TokenType::FLOAT);
    bool ExpectVariable = Match(TokenType::VARIABLE);
    bool ExpectString = Match(TokenType::STRING);
    bool ExpectArithmeticOp = Next().GetType() == TokenType::ADD || Next().GetType() == TokenType::SUBSTRACT 
    || Next().GetType() == TokenType::MULTIPLY || Next().GetType() == TokenType::DIVIDE;
    bool ExpectOperand = ExpectInt || ExpectFloat || ExpectVariable || ExpectString || ExpectArithmeticOp;
    bool ExpectSemicolon = Match(TokenType::SEMICOLON);
    int count = 0;
    bool Check = ExpectOperand;
    while(Check)
    {
        if(ExpectArithmeticOp) {Print->Contents.push_back(ParseArithmeticOp());}
        else if(ExpectString) {Print->Contents.push_back(ParseString());}
        else if(ExpectInt | ExpectFloat | ExpectVariable) {Print->Contents.push_back(ParseOperand());}
        else {Eat(TokenType::SEMICOLON);}
        count++;
        ExpectInt = Match(TokenType::INT);
        ExpectFloat = Match(TokenType::FLOAT);
        ExpectVariable = Match(TokenType::VARIABLE);
        ExpectString = Match(TokenType::STRING);
        ExpectArithmeticOp = Next().GetType() == TokenType::ADD || Next().GetType() == TokenType::SUBSTRACT 
        || Next().GetType() == TokenType::MULTIPLY || Next().GetType() == TokenType::DIVIDE;
        ExpectOperand = ExpectInt || ExpectFloat || ExpectVariable || ExpectString || ExpectArithmeticOp;
        ExpectSemicolon = Match(TokenType::SEMICOLON);
        Check = (ExpectOperand && count % 2 == 0) || (ExpectSemicolon && count % 2 == 1);
    }
    debug::ExtendedLog("ParsePrint ENDED");
    return Print;
}

std::unique_ptr<ASTNode> Parser::ParseStop()
{
    debug::log("ParseStop");
    Eat(TokenType::STOP);
    return std::make_unique<StopNode>();
}

std::unique_ptr<ASTNode> Parser::ParseLet()
{
    debug::log("ParseLet");
    auto Let = std::make_unique<LetNode>();
    Eat(TokenType::LET);
    Let->Expression = ParseExpression();
    return Let;
}

std::unique_ptr<ASTNode> Parser::ParseVariable()
{
    debug::log("ParseVariable");
    auto Variable = std::make_unique<VariableNode>();
    Variable->Name = Eat(TokenType::VARIABLE).GetValue();
    return Variable;
}

std::unique_ptr<ASTNode> Parser::ParseInt()
{
    debug::log("ParseInt");
    auto Int = std::make_unique<IntNode>();
    Int->Value = Eat(TokenType::INT).GetValue();
    return Int;
}

std::unique_ptr<ASTNode> Parser::ParseFloat()
{
    debug::log("ParseFloat");
    auto Float = std::make_unique<FloatNode>();
    Float->Value = Eat(TokenType::FLOAT).GetValue();
    return Float;
}

std::unique_ptr<ASTNode> Parser::ParseString()
{
    debug::log("ParseString");
    auto String = std::make_unique<StringNode>();
    String->Value = Eat(TokenType::STRING).GetValue();
    return String;
}

std::unique_ptr<ASTNode> Parser::ParseArithmeticOp()
{
    debug::log("ParseArithmeticOp");
    auto ArithmeticOp = std::make_unique<ArithmeticOpNode>();
    bool ExpectAdd = Match(TokenType::ADD);
    bool ExpectSubstract = Match(TokenType::SUBSTRACT);
    bool ExpectMultiply = Match(TokenType::MULTIPLY);
    bool ExpectDivide = Match(TokenType::DIVIDE);
    bool ExpectInt = Match(TokenType::INT);
    bool ExpectFloat = Match(TokenType::FLOAT);
    bool ExpectVariable = Match(TokenType::VARIABLE);
    bool ExpectOperation = ExpectAdd || ExpectSubstract || ExpectMultiply || ExpectDivide;
    bool ExpectOperand = ExpectInt || ExpectFloat || ExpectVariable;
    bool Check = ExpectOperand;
    int count = 0;
    while(Check)
    {
        if(ExpectOperand) {ArithmeticOp->Operands.push_back(ParseOperand());}
        else if(ExpectAdd) {ArithmeticOp->Operations.push_back('+'); Iterate();}
        else if(ExpectSubstract) {ArithmeticOp->Operations.push_back('-'); Iterate();}
        else if(ExpectMultiply) {ArithmeticOp->Operations.push_back('*'); Iterate();}
        else if(ExpectDivide) {ArithmeticOp->Operations.push_back('/'); Iterate();}
        count++;
        ExpectAdd = Match(TokenType::ADD);
        ExpectSubstract = Match(TokenType::SUBSTRACT);
        ExpectMultiply = Match(TokenType::MULTIPLY);
        ExpectDivide = Match(TokenType::DIVIDE);
        ExpectInt = Match(TokenType::INT);
        ExpectFloat = Match(TokenType::FLOAT);
        ExpectVariable = Match(TokenType::VARIABLE);
        ExpectOperation = ExpectAdd || ExpectSubstract || ExpectMultiply || ExpectDivide;
        ExpectOperand = ExpectInt || ExpectFloat || ExpectVariable;
        Check = (count % 2 == 0 && ExpectOperand) || (count % 2 == 1 && ExpectOperation);
    }
    return ArithmeticOp;
}