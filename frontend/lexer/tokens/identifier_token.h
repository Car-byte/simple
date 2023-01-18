#ifndef SIMPLE_IDENTIFIER_TOKEN_H
#define SIMPLE_IDENTIFIER_TOKEN_H

#include "token.h"
#include "token_type.h"

class IdentifierToken: public Token {
public:
    IdentifierToken(const std::string& identifier)
        : Token(TokenType::Identifier, identifier) {}

    virtual ~IdentifierToken() override = default;
};

#endif //SIMPLE_IDENTIFIER_TOKEN_H
