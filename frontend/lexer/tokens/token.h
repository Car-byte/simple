#ifndef SIMPLE_TOKEN_H
#define SIMPLE_TOKEN_H

#include <string>

#include "token_type.h"

class Token {
public:
    Token(TokenType tokenType, const std::string& rawVal)
        : tokenType(tokenType), rawVal(rawVal) {}

    virtual ~Token() = default;

    TokenType getTokenType() const {
        return tokenType;
    }

    std::string getRawVal() const {
        return rawVal;
    }

    bool isTokenType(TokenType compareTokenType) const {
        return tokenType == compareTokenType;
    }

private:
    std::string rawVal;
    TokenType tokenType;
};

#endif