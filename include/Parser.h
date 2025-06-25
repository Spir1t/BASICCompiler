#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Token.h"
#include "AbstractSyntaxTree.h"

namespace Parser
{
    //Parsing
    std::unique_ptr<ProgramNode> ParseProgram(std::vector<Token>& NewTokens);
    std::unique_ptr<ASTNode> ParseBlock();
    std::unique_ptr<ASTNode> ParseLine();
    std::unique_ptr<ASTNode> ParseWhile();
    std::unique_ptr<ASTNode> ParseExpression();
    std::unique_ptr<ASTNode> ParseBasicFunction();
    std::unique_ptr<ASTNode> ParseOperand();
    std::unique_ptr<ASTNode> ParsePrint();
    std::unique_ptr<ASTNode> ParseStop();
    std::unique_ptr<ASTNode> ParseLet();
    std::unique_ptr<ASTNode> ParseVariable();
    std::unique_ptr<ASTNode> ParseInt();
    std::unique_ptr<ASTNode> ParseFloat();
    std::unique_ptr<ASTNode> ParseString();
    std::unique_ptr<ASTNode> ParseArithmeticOp();
    std::unique_ptr<ASTNode> ParseConditionalOp();
    std::unique_ptr<ASTNode> ParseIfCond();
    std::unique_ptr<ASTNode> ParseIfElseCond();
    std::unique_ptr<ASTNode> ParseIf();
    std::unique_ptr<ASTNode> ParseThen();
    std::unique_ptr<ASTNode> ParseElse();
    std::unique_ptr<ASTNode> ParseCondition();
    std::unique_ptr<ASTNode> ParseStatement();
    std::string ParseComparison();


    //Variables
    inline std::vector<Token> Tokens;
    inline std::vector<std::string> Headers;
    inline size_t Pos = 0;

    //Utils
    Token Eat(TokenType Type);
    Token Current();
    Token Next();
    Token NextAfterNext();
    inline void Iterate(){Pos++;}
    inline TokenType CurrentType(){return Current().GetType();}
    void Expect(TokenType Type);
    void Expect(std::vector<TokenType> Types);
    inline bool Match(TokenType Type){return CurrentType() == Type;}
}