#ifndef SIMPLE_KEYWORD_TOKEN_H
#define SIMPLE_KEYWORD_TOKEN_H

#include<string>

#include "token.h"
#include "token_type.h"
#include "keyword_type.h"

class KeywordToken: public Token {
public:
    KeywordToken(KeywordType keywordType, const std::string& rawVal)
        : Token(TokenType::Keyword, rawVal), keywordType(keywordType) {}

    KeywordType getKeywordType() const {
        return keywordType;
    }

    virtual ~KeywordToken() override = default;

private:
    KeywordType keywordType;
};

#endif //SIMPLE_KEYWORD_TOKEN_H
