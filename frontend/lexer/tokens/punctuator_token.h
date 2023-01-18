#ifndef SIMPLE_PUNCTUATOR_TOKEN_H
#define SIMPLE_PUNCTUATOR_TOKEN_H

#include <string>

#include "token.h"
#include "token_type.h"
#include "punctuator_type.h"

class PunctuatorToken: public Token {
public:

    PunctuatorToken(PunctuatorType punctuatorType, const std::string& rawVal)
        : Token(TokenType::Punctuator, rawVal), punctuatorType(punctuatorType) {}

    PunctuatorType getPunctuatorType() const {
        return punctuatorType;
    }

    bool isTerminationPunctuator() const {
        return getPunctuatorType() == PunctuatorType::CloseParenthesis ||
            getPunctuatorType() == PunctuatorType::SemiColon ||
            getPunctuatorType() == PunctuatorType::Comma;
    }

    virtual ~PunctuatorToken() override = default;

private:
    PunctuatorType punctuatorType;
};

#endif //SIMPLE_PUNCTUATOR_TOKEN_H
