#include "lexer.h"

#include "./tokens/operator_token.h"
#include "./tokens/operator_type.h"
#include "./tokens/punctuator_token.h"
#include "./tokens/punctuator_type.h"
#include "./tokens/literal_number_token.h"
#include "./tokens/keyword_token.h"
#include "./tokens/keyword_type.h"
#include "./tokens/identifier_token.h"

#include <vector>
#include <memory>

std::vector<std::shared_ptr<Token>> Lexer::tokenize() {
    while (!src.atEnd()) {
        if (std::isspace(src.peek())) {
            src.advance();
        }
        else {
            tokens.push_back(getNextToken());
        }
    }

    return std::move(tokens);
}

std::shared_ptr<Token> Lexer::getNextToken() {
    switch (src.peek()) {
        case '+':
            return std::make_shared<OperatorToken>(OperatorType::Add, std::string(1, src.advance()));
        case '-':
            src.advance();

            if (src.peek() == '>') {
                src.advance();
                return std::make_shared<PunctuatorToken>(PunctuatorType::Arrow, "->");
            }

            return std::make_shared<OperatorToken>(OperatorType::Sub, "-");
        case '=':
            src.advance();

            if (src.peek() == '=') {
                src.advance();
                return std::make_shared<OperatorToken>(OperatorType::LogicalEqual, "==");
            }

            return std::make_shared<PunctuatorToken>(PunctuatorType::Equal, "=");
        case '>':
            src.advance();

            if (src.peek() == '=') {
                src.advance();
                return std::make_shared<OperatorToken>(OperatorType::GreaterEqual, ">=");
            }

            return std::make_shared<OperatorToken>(OperatorType::Greater, ">");
        case '<':
            src.advance();

            if (src.peek() == '=') {
                src.advance();
                return std::make_shared<OperatorToken>(OperatorType::LesserEqual, "<=");
            }

            return std::make_shared<OperatorToken>(OperatorType::Lesser, "<");
        case '!':
            src.advance();

            if (src.peek() == '=') {
                src.advance();
                return std::make_shared<OperatorToken>(OperatorType::NotLogicalEqual, "!=");
            }

            assert(false && "unidentified token: !");
        case ';':
            return std::make_shared<PunctuatorToken>(PunctuatorType::SemiColon, std::string(1, src.advance()));
        case ':':
            return std::make_shared<PunctuatorToken>(PunctuatorType::Colon, std::string(1, src.advance()));
        case '(':
            return std::make_shared<PunctuatorToken>(PunctuatorType::OpenParenthesis, std::string(1, src.advance()));
        case ')':
            return std::make_shared<PunctuatorToken>(PunctuatorType::CloseParenthesis, std::string(1, src.advance()));
        case '{':
            return std::make_shared<PunctuatorToken>(PunctuatorType::OpenCurlyBrace, std::string(1, src.advance()));
        case '}':
            return std::make_shared<PunctuatorToken>(PunctuatorType::CloseCurlyBrace, std::string(1, src.advance()));
        case ',':
            return std::make_shared<PunctuatorToken>(PunctuatorType::Comma, std::string(1, src.advance()));
        case '0'...'9':
            return getNumberLiteralToken();
        default:
            return getIdentifierOrKeywordToken();
    }
}

std::shared_ptr<Token> Lexer::getNumberLiteralToken() {
    std::string litStr;

    assert(isnumber(src.peek()));

    bool isFloat = false;

    while (isnumber(src.peek()) || src.peek() == '.') {
        if (src.peek() == '.') {
            assert(!isFloat);
            isFloat = true;
        }

        litStr.push_back(src.advance());
    }

    assert(litStr.back() != '.');

    return std::make_shared<LiteralNumberToken>(litStr);
}

std::shared_ptr<Token> Lexer::getIdentifierOrKeywordToken() {
    std::string identifier;

    assert(std::isalpha(src.peek()) || src.peek() == '_');

    while (std::isalnum(src.peek()) || src.peek() == '_') {
        identifier.push_back(src.advance());
    }

    if (keyWordMap.contains(identifier)) {
        return std::make_shared<KeywordToken>(keyWordMap.at(identifier), identifier);
    }

    return std::make_shared<IdentifierToken>(identifier);
}