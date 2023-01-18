#ifndef SIMPLE_TOKEN_SLIDING_WINDOW_H
#define SIMPLE_TOKEN_SLIDING_WINDOW_H

#include <vector>
#include <memory>
#include <span>

#include "../lexer/tokens/token.h"
#include "../lexer/tokens/token_type.h"
#include "../lexer/tokens/punctuator_type.h"
#include "../lexer/tokens/operator_type.h"
#include "../lexer/tokens/keyword_type.h"
#include "sliding_window.h"


class TokenSlidingWindow: public SlidingWindow<std::shared_ptr<Token>> {
public:
    TokenSlidingWindow(std::span<std::shared_ptr<Token>> tokens)
        : SlidingWindow<std::shared_ptr<Token>>(tokens) {}

    virtual ~TokenSlidingWindow() = default;

    bool isNextIdentifier();
    bool isNextPunctuator(PunctuatorType punctuatorType);
    bool isNextOperator(OperatorType operatorType);
    bool isNextKeyword(KeywordType keywordType);
    bool isNextNumberLiteral();

    bool isTerminationPunctuator();

    void advancePunctuator(PunctuatorType punctuatorType);
    void advanceOperator(OperatorType operatorType);
    void advanceKeyword(KeywordType keywordType);
    void advanceIdentifier();
    void advanceNumberLiteral();
    void advanceOperator();

private:
    bool isNext(TokenType tokenType);
};

#endif //SIMPLE_TOKEN_SLIDING_WINDOW_H
