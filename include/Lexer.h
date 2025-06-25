#pragma once
#include <string>
#include <vector>
#include "Token.h"

namespace Lexer
{
    std::vector<Token> GetTokens(std::string& src);
}