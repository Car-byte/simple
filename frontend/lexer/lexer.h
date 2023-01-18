#ifndef SIMPLE_LEXER_H
#define SIMPLE_LEXER_H

#include <string>
#include <vector>
#include <memory>

#include "./tokens/token.h"
#include "../utils/sliding_window.h"

class Lexer {
public:
    Lexer(const SlidingWindow<char>& src): src(src) {}

    std::vector<std::shared_ptr<Token>> tokenize();

private:
    SlidingWindow<char> src;
    std::vector<std::shared_ptr<Token>> tokens;

    std::shared_ptr<Token> getNextToken();
    std::shared_ptr<Token> getNumberLiteralToken();
    std::shared_ptr<Token> getIdentifierOrKeywordToken();
};


#endif