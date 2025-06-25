#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include "include/Lexer.h"
#include "include/Token.h"
#include "include/Parser.h"
#include "include/AbstractSyntaxTree.h"
#include "include/Compiler.h"

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        throw("Build error: no file selected");
    }
    std::ifstream File(argv[1]);
    std::ostringstream buffer;
    buffer << File.rdbuf();
    std::string src = buffer.str();

    std::vector<Token> Tokens = Lexer::GetTokens(src);
    std::string Program = Parser::ParseProgram(Tokens)->ConvertToC();
    if(argc == 4 && std::string(argv[2]) == "-o")
    {
        Compiler::Compile(Program, argv[3]);
        std::cout << argv[3] << std::endl;
        return 0;
    }
    Compiler::Compile(Program);
    return 0;
}