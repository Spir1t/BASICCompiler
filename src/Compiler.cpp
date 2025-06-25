#include <string>
#include <cstdlib>
#include <fstream>
#include "../include/Compiler.h"

void Compiler::Compile(std::string Program)
{
    std::ofstream Out("Out.c");
    Out << Program;
    Out.close();

    system("g++ Out.c -o output.exe");
    system("del Out.c");
}

void Compiler::Compile(std::string Program, std::string Output)
{
    std::ofstream Out(Output + ".c");
    Out << Program;
    Out.close();
    std::string SystemCall = "g++ Out.c -o " + Output;
    system(SystemCall.c_str());
    system("del Out.c");
}