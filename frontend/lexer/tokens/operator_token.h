#ifndef SIMPLE_OPERATOR_TOKEN_H
#define SIMPLE_OPERATOR_TOKEN_H

#include <string>

#include "token_type.h"
#include "token.h"
#include "operator_type.h"

class OperatorToken: public Token {
public:
    OperatorToken(OperatorType operatorType, const std::string& rawVal)
        : Token(TokenType::Operator, rawVal), operatorType(operatorType) {}

    OperatorType getOperatorType() const {
        return operatorType;
    }

    int getOperatorPrecedents() const {
        switch (getOperatorType()) {
            case OperatorType::LogicalEqual:
            case OperatorType::NotLogicalEqual:
            case OperatorType::Greater:
            case OperatorType::GreaterEqual:
            case OperatorType::Lesser:
            case OperatorType::LesserEqual:
                return 20;
            case OperatorType::Add:
            case OperatorType::Sub:
                return 50;
        }
    }

    virtual ~OperatorToken() override = default;

private:
    OperatorType operatorType;
};

#endif //SIMPLE_OPERATOR_TOKEN_H
