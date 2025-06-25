#pragma once
#include <string>

enum TokenType
{
    INT = 0,        //0
    FLOAT,          //1
    STRING,         //2
    LET,            //3
    EQUAL,          //4
    LESS,           //5
    MORE,           //6
    LESS_EQUAL,     //7
    MORE_EQUAL,     //8
    NOT_EQUAL,      //9
    ADD,            //10
    SUBSTRACT,      //11
    MULTIPLY,       //12
    DIVIDE,         //13
    VARIABLE,       //14
    PRINT,          //15
    STOP,           //16
    IF,             //17
    ELSE,           //18
    THEN,           //19
    AND,            //20
    OR,             //21
    NOT,            //22
    WHILE,          //23
    REPEAT,         //24
    SEMICOLON,      //25
    ENDWHILE,       //26
    EndOfFile       //27
};

class Token
{
    public:
    Token() = default;
    Token(TokenType Type, std::string Value) : Type{Type}, Value{Value} {}
    Token(std::string Value);
    TokenType GetType() {return Type;}
    std::string GetValue() {return Value;}

    private:
    TokenType Type;
    std::string Value;
};

std::string Type2String(TokenType Type);