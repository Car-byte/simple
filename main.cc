#include <iostream>
#include <vector>
#include <string>
#include <span>
#include <fstream>

#include "frontend/lexer/lexer.h"
#include "frontend/utils/sliding_window.h"
#include "frontend/parser/parser.h"

#include "backend/code_generator.h"

std::string readFile(const std::string& filePath) {
    std::ifstream inputFile(filePath);
    assert(!inputFile.fail());

    return std::string((std::istreambuf_iterator<char>(inputFile)),
                           std::istreambuf_iterator<char>());
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "no file to compile";
        return 1;
    }

    std::string fileContents = readFile(argv[1]);

    SlidingWindow<char> inputWindow(fileContents);
    Lexer lexer(inputWindow);
    auto tokens = lexer.tokenize();

    TokenSlidingWindow tokenWindow(tokens);
    Parser parser(tokenWindow);
    Program program = parser.parse();

    program.setShouldRunOptimizations(false);

    CodeGenerator codeGenerator(program);
    codeGenerator.codeGenProgram();

    codeGenerator.dumpIr();

    return 0;
}