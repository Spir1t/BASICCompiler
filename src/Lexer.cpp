#include <vector>
#include <string>
#include <iostream>
#include "../include/Lexer.h"
#include "../include/Token.h"

namespace
{
    bool QuotesOpen;
    std::vector<std::string> Split(std::string src)
    {
        std::vector<std::string> out;
        std::string word;
        for(auto s : src)
        {
            if(s == '\n')
            {
                if(!word.empty())
                {
                    out.push_back(word);
                    word.clear();
                }
            }
            else if(s == ' ' && !QuotesOpen)
            {
                if(!word.empty())
                {
                    out.push_back(word);
                    word.clear();
                }
            }
            else if(s == ';' && !QuotesOpen)
            {
                out.push_back(word);
                word.clear();
                out.push_back(";");
            }
            else
            {
                if(s == '"' && QuotesOpen) QuotesOpen = false;
                else if(s == '"' && !QuotesOpen) QuotesOpen = true;
                word.push_back(s);
            }
        }
        out.push_back(word);
        out.push_back("\0");
        return out;
    }
}

std::vector<Token> Lexer::GetTokens(std::string& src)
{
    std::vector<Token> Out;
    std::vector<std::string> Words(Split(src));
    for(auto Word : Words)
    {
        Out.push_back(Token(Word));
    }
    return Out;
}