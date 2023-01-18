#include "token_sliding_window.h"

#include "../lexer/tokens/token.h"
#include "../lexer/tokens/token_type.h"
#include "../lexer/tokens/punctuator_type.h"
#include "../lexer/tokens/punctuator_token.h"
#include "../lexer/tokens/operator_type.h"
#include "../lexer/tokens/operator_token.h"
#include "../lexer/tokens/keyword_type.h"
#include "../lexer/tokens/keyword_token.h"

bool TokenSlidingWindow::isNext(TokenType tokenType) {
    return peek()->isTokenType(tokenType);
}

void TokenSlidingWindow::advanceKeyword(KeywordType keywordType) {
    assert(isNext(TokenType::Keyword));
    assert(std::dynamic_pointer_cast<KeywordToken>(peek())->getKeywordType() == keywordType);
    advance();
}

void TokenSlidingWindow::advanceOperator(OperatorType operatorType) {
    assert(isNext(TokenType::Operator));
    assert(std::dynamic_pointer_cast<OperatorToken>(peek())->getOperatorType() == operatorType);
    advance();
}

void TokenSlidingWindow::advanceIdentifier() {
    assert(isNext(TokenType::Identifier));
    advance();
}

void TokenSlidingWindow::advancePunctuator(PunctuatorType punctuatorType) {
    assert(isNext(TokenType::Punctuator));
    assert(std::dynamic_pointer_cast<PunctuatorToken>(peek())->getPunctuatorType() == punctuatorType);
    advance();
}

void TokenSlidingWindow::advanceNumberLiteral() {
    assert(isNext(TokenType::Literal));
    advance();
}

void TokenSlidingWindow::advanceOperator() {
    assert(isNext(TokenType::Operator));
    advance();
}

bool TokenSlidingWindow::isNextIdentifier() {
    return isNext(TokenType::Identifier);
}

bool TokenSlidingWindow::isNextPunctuator(PunctuatorType punctuatorType) {
    if (!isNext(TokenType::Punctuator)) return false;
    return std::dynamic_pointer_cast<PunctuatorToken>(peek())->getPunctuatorType() == punctuatorType;
}

bool TokenSlidingWindow::isNextOperator(OperatorType operatorType) {
    if (!isNext(TokenType::Operator)) return false;
    return std::dynamic_pointer_cast<OperatorToken>(peek())->getOperatorType() == operatorType;
}

bool TokenSlidingWindow::isNextKeyword(KeywordType keywordType) {
    if (!isNext(TokenType::Keyword)) return false;
    return std::dynamic_pointer_cast<KeywordToken>(peek())->getKeywordType() == keywordType;
}

bool TokenSlidingWindow::isNextNumberLiteral() {
    return isNext(TokenType::Literal);
}

bool TokenSlidingWindow::isTerminationPunctuator() {
    if (!isNext(TokenType::Punctuator)) return false;
    return std::dynamic_pointer_cast<PunctuatorToken>(peek())->isTerminationPunctuator();
}
