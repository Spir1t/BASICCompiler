#include "../include/Token.h"
#include <stdexcept>
#include <algorithm>

static const std::string TokenTypeNames[] = {
    "INT",          // 0
    "FLOAT",        // 1
    "STRING",       // 2
    "LET",          // 3
    "EQUAL",        // 4
    "LESS",         // 5
    "MORE",         // 6
    "LESS_EQUAL",   // 7
    "MORE_EQUAL",   // 8
    "NOT_EQUAL",    // 9
    "ADD",          // 10
    "SUBSTRACT",    // 11
    "MULTIPLY",     // 12
    "DIVIDE",       // 13
    "VARIABLE",     // 14
    "PRINT",        // 15
    "STOP",         // 16
    "IF",           // 17
    "ELSE",         // 18
    "THEN",         // 19
    "AND",          // 20
    "OR",           // 21
    "NOT",          // 22
    "WHILE",        // 23
    "REPEAT",       // 24
    "SEMICOLON",    // 25
    "ENDWHILE",     // 26
    "EndOfFile"     // 27
};

static bool OnlyDigits(std::string Value)
{
    for(char c : Value)
    {
        if(!isdigit(c) && c != '.') return false;
    }
    return true;
}

Token::Token(std::string Value)
{
    if (Value == "LET")
    {
        Type = TokenType::LET;
    }
    else if (Value == "PRINT")
    {
        Type = TokenType::PRINT;
    }
    else if (Value == "STOP")
    {
        Type = TokenType::STOP;
    }
    else if (Value == "IF")
    {
        Type = TokenType::IF;
    }
    else if (Value == "ELSE")
    {
        Type = TokenType::ELSE;
    }
    else if (Value == "THEN")
    {
        Type = TokenType::THEN;
    }
    else if (Value == "AND")
    {
        Type = TokenType::AND;
    }
    else if (Value == "OR")
    {
        Type = TokenType::OR;
    }
    else if (Value == "NOT")
    {
        Type = TokenType::NOT;
    }
    else if (Value == "WHILE")
    {
        Type = TokenType::WHILE;
    }
    else if (Value == "REPEAT")
    {
        Type = TokenType::REPEAT;
    }
    else if (Value == "ENDWHILE")
    {
        Type = TokenType::ENDWHILE;
    }
    else if (Value == "=")
    {
        Type = TokenType::EQUAL;
    }
    else if (Value == "<")
    {
        Type = TokenType::LESS;
    }
    else if (Value == ">")
    {
        Type = TokenType::MORE;
    }
    else if (Value == "<=")
    {
        Type = TokenType::LESS_EQUAL;
    }
    else if (Value == ">=")
    {
        Type = TokenType::MORE_EQUAL;
    }
    else if (Value == "<>")
    {
        Type = TokenType::NOT_EQUAL;
    }
    else if (Value == "+")
    {
        Type = TokenType::ADD;
    }
    else if (Value == "-")
    {
        Type = TokenType::SUBSTRACT;
    }
    else if (Value == "*")
    {
        Type = TokenType::MULTIPLY;
    }
    else if (Value == "/")
    {
        Type = TokenType::DIVIDE;
    }
    else if(Value == ";")
    {
        Type = TokenType::SEMICOLON;
    }
    else if (Value == "\0")
    {
        Type = TokenType::EndOfFile;
    }
    else if (Value.find('"') != std::string::npos)
    {
        Type = TokenType::STRING;
        Value = Value.substr(1, Value.size() - 2);
    }
    else if (OnlyDigits(Value))
    {
        if (Value.find('.') != std::string::npos)
            Type = TokenType::FLOAT;
        else
            Type = TokenType::INT;
    }
    else if (!isdigit(Value[0]))
    {
        Type = TokenType::VARIABLE;
    }
    else
    {
        throw std::runtime_error("Incorrect Token: " + Value);
    }

    this->Value = Value;
}

std::string Type2String(TokenType Type)
{
    return(TokenTypeNames[Type]);
}