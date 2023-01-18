#ifndef SIMPLE_LITERAL_NUMBER_TOKEN_H
#define SIMPLE_LITERAL_NUMBER_TOKEN_H

#include <string>

#include "token.h"
#include "token_type.h"

class LiteralNumberToken: public Token {
public:
    LiteralNumberToken(const std::string& rawVal)
        : Token(TokenType::Literal, rawVal) {}

    virtual ~LiteralNumberToken() override = default;
};

#endif //SIMPLE_LITERAL_NUMBER_TOKEN_H
