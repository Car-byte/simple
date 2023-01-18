#include "parser.h"

#include <iostream>
#include <vector>
#include <string>
#include <memory>

#include "../lexer/tokens/token.h"
#include "../lexer/tokens/operator_token.h"

#include "../../backend/program.h"

#include "./ast/ast_function.h"
#include "./ast/ast_var_declaration.h"
#include "./ast/ast_function_call.h"
#include "./ast/ast_binary_op.h"
#include "./ast/ast_assignment.h"
#include "./ast/ast_function_params.h"
#include "./ast/ast_variable.h"
#include "./ast/ast_literal_int32.h"
#include "./ast/ast_literal_float32.h"
#include "./ast/ast_function_return.h"
#include "./ast/ast_if_else.h"

Program Parser::parse() {
    std::vector<std::shared_ptr<AstFunction>> functions;
    std::shared_ptr<AstFunction> mainFunc;

    bool hasMain = false;

    while (!tokenWindow.atEnd()) {
        std::shared_ptr<AstFunction> func = parseFunction();

        if (func->getIdentifier() == "main") {
            assert(!hasMain);
            hasMain = true;
            mainFunc = func;
        }
        else {
            functions.push_back(func);
        }
    }

    return Program(functions, mainFunc);
}

int Parser::getPrecedents(const std::shared_ptr<Token>& token) {
    auto opToken = std::dynamic_pointer_cast<OperatorToken>(token);

    if (opToken) {
        return opToken->getOperatorPrecedents();
    }

    return -1;
}

std::shared_ptr<AstFunction> Parser::parseFunction() {
    tokenWindow.advanceKeyword(KeywordType::Func);

    std::string identifier = tokenWindow.peek()->getRawVal();
    tokenWindow.advanceIdentifier();

    auto params = parseFunctionParams();

    // default return type is void if not specified
    std::string typeStr = "void";

    if (tokenWindow.isNextPunctuator(PunctuatorType::Arrow)) {
        tokenWindow.advancePunctuator(PunctuatorType::Arrow);

        typeStr = tokenWindow.peek()->getRawVal();
        tokenWindow.advanceIdentifier();
    }

    std::shared_ptr<Type> returnType = typeFactory.getType(typeStr);

    auto funcBody = parseBody();

    return std::make_shared<AstFunction>(identifier, params, funcBody, returnType);
}

std::vector<AstFunctionParam> Parser::parseFunctionParams() {
    tokenWindow.advancePunctuator(PunctuatorType::OpenParenthesis);

    std::vector<AstFunctionParam> params;
    bool expectedNextParam = false;

    while (!tokenWindow.isNextPunctuator(PunctuatorType::CloseParenthesis)) {
        std::string identifier = tokenWindow.peek()->getRawVal();
        tokenWindow.advanceIdentifier();
        tokenWindow.advancePunctuator(PunctuatorType::Colon);

        std::string typeStr = tokenWindow.peek()->getRawVal();
        tokenWindow.advanceIdentifier();

        auto paramType = typeFactory.getType(typeStr);
        assert(!paramType->isVoidType());

        if (tokenWindow.isNextPunctuator(PunctuatorType::Comma)) {
            expectedNextParam = true;
            tokenWindow.advancePunctuator(PunctuatorType::Comma);
        }
        else {
            expectedNextParam = false;
        }

        params.emplace_back(identifier, paramType);
    }

    assert(!expectedNextParam);

    tokenWindow.advancePunctuator(PunctuatorType::CloseParenthesis);

    return params;
}

std::vector<std::shared_ptr<AstNode>> Parser::parseBody() {
    std::vector<std::shared_ptr<AstNode>> body;

    tokenWindow.advancePunctuator(PunctuatorType::OpenCurlyBrace);

    while (!tokenWindow.isNextPunctuator(PunctuatorType::CloseCurlyBrace)) {
        if (tokenWindow.isNextKeyword(KeywordType::Var)) {
            body.push_back(parseVarDeclaration());
        }
        else if (tokenWindow.isNextIdentifier()) {
            body.push_back(parseVarAssignmentOrFuncCall());
        }
        else if (tokenWindow.isNextKeyword(KeywordType::If)) {
            body.push_back(parseIfElse());
        }
        else if (tokenWindow.isNextKeyword(KeywordType::Return)) {
            body.push_back(parseReturn());
        }
        else {
            assert(false && "unknown token in top level of body");
        }
    }

    tokenWindow.advancePunctuator(PunctuatorType::CloseCurlyBrace);

    return body;
}

std::shared_ptr<AstNode> Parser::parseVarAssignmentOrFuncCall() {
    std::string identifier = tokenWindow.peek()->getRawVal();
    tokenWindow.advanceIdentifier();

    std::shared_ptr<AstNode> node;

    if (tokenWindow.isNextPunctuator(PunctuatorType::OpenParenthesis)) {
        node = parseFunctionCall(identifier);
    }
    else {
        tokenWindow.advancePunctuator(PunctuatorType::Equal);
        auto assigned = parseExpression();
        node = std::make_shared<AstAssignment>(identifier, assigned);
    }

    tokenWindow.advancePunctuator(PunctuatorType::SemiColon);

    return node;
}

std::shared_ptr<AstNode> Parser::parseVarDeclaration() {
    tokenWindow.advanceKeyword(KeywordType::Var);

    std::string identifier = tokenWindow.peek()->getRawVal();
    tokenWindow.advanceIdentifier();

    tokenWindow.advancePunctuator(PunctuatorType::Colon);

    std::string rawType = tokenWindow.peek()->getRawVal();
    tokenWindow.advanceIdentifier();
    auto varType = typeFactory.getType(rawType);

    tokenWindow.advancePunctuator(PunctuatorType::Equal);

    auto assignedExpr = parseExpression();
    tokenWindow.advancePunctuator(PunctuatorType::SemiColon);

    return std::make_shared<AstVarDeclaration>(identifier, varType, assignedExpr);
}

std::shared_ptr<AstNode> Parser::parseExpression() {
    std::shared_ptr<AstNode> lhs = parsePrimaryExpression();

    return parseBinaryOpExpression(lhs, 0);
}

std::shared_ptr<AstNode> Parser::parsePrimaryExpression() {
    if (tokenWindow.isNextNumberLiteral()) {
        return parseNumberLiteral();
    }
    else if (tokenWindow.isNextIdentifier()) {
        return parseFuncCallOrVariable();
    }
    else if (tokenWindow.isNextPunctuator(PunctuatorType::OpenParenthesis)) {
        return parseParenthesesExpression();
    }
    else {
        assert(false && "unknown primary expression token");
    }
}

std::shared_ptr<AstNode> Parser::parseFuncCallOrVariable() {
    std::string identifier = tokenWindow.peek()->getRawVal();
    tokenWindow.advanceIdentifier();

    if (tokenWindow.isNextPunctuator(PunctuatorType::OpenParenthesis)) {
        return parseFunctionCall(identifier);
    }
    else {
        return parseVariable(identifier);
    }
}

std::shared_ptr<AstNode> Parser::parseNumberLiteral() {
    std::string litVal = tokenWindow.peek()->getRawVal();
    tokenWindow.advanceNumberLiteral();

    if (litVal.find('.') != std::string::npos) {
        return std::make_shared<AstLiteralFloat32>(litVal);
    }
    else {
        return std::make_shared<AstLiteralInt32>(litVal);
    }
}

std::shared_ptr<AstNode> Parser::parseBinaryOpExpression(std::shared_ptr<AstNode> lhs, int precedents) {
    while (true) {
        int curPrecedents = getPrecedents(tokenWindow.peek());

        if (curPrecedents < precedents || tokenWindow.isTerminationPunctuator()) {
            return lhs;
        }

        auto opToken = tokenWindow.peek();
        tokenWindow.advanceOperator();
        OperatorType op = std::dynamic_pointer_cast<OperatorToken>(opToken)->getOperatorType();

        auto rhs = parsePrimaryExpression();
        int nextPrecedents = getPrecedents(tokenWindow.peek());

        if (curPrecedents < nextPrecedents) {
            rhs = parseBinaryOpExpression(rhs, nextPrecedents + 1);
        }

        lhs = std::make_shared<AstBinaryOp>(op, lhs, rhs);
    }
}

std::shared_ptr<AstNode> Parser::parseParenthesesExpression() {
    tokenWindow.advancePunctuator(PunctuatorType::OpenParenthesis);

    assert(!tokenWindow.isNextPunctuator(PunctuatorType::CloseParenthesis));
    auto node = parseExpression();

    tokenWindow.advancePunctuator(PunctuatorType::CloseParenthesis);

    return node;
}

std::shared_ptr<AstNode> Parser::parseFunctionCall(const std::string& identifier) {
    tokenWindow.advancePunctuator(PunctuatorType::OpenParenthesis);

    std::vector<std::shared_ptr<AstNode>> args;
    bool expectedNextParam = false;

    while (!tokenWindow.isNextPunctuator(PunctuatorType::CloseParenthesis)) {
        args.push_back(parseExpression());

        if (tokenWindow.isNextPunctuator(PunctuatorType::Comma)) {
            expectedNextParam = true;
            tokenWindow.advancePunctuator(PunctuatorType::Comma);
        }
        else {
            expectedNextParam = false;
        }
    }

    assert(!expectedNextParam);
    tokenWindow.advancePunctuator(PunctuatorType::CloseParenthesis);

    return std::make_shared<AstFunctionCall>(identifier, args);
}

std::shared_ptr<AstNode> Parser::parseVariable(const std::string& identifier) {
    return std::make_shared<AstVariable>(identifier);
}

std::shared_ptr<AstNode> Parser::parseReturn() {
    tokenWindow.advanceKeyword(KeywordType::Return);

    auto returnVal = parseExpression();

    tokenWindow.advancePunctuator(PunctuatorType::SemiColon);

    return std::make_shared<AstFunctionReturn>(returnVal);
}

std::shared_ptr<AstNode> Parser::parseIfElse() {
    tokenWindow.advanceKeyword(KeywordType::If);

    auto ifCond = parseIfCond();
    auto ifBody = parseBody();

    std::vector<std::shared_ptr<AstNode>> elseBody;

    if (tokenWindow.isNextKeyword(KeywordType::Else)) {
        tokenWindow.advanceKeyword(KeywordType::Else);
        elseBody = parseBody();
    }

    return std::make_shared<AstIfElse>(ifCond, ifBody, elseBody);
}

std::shared_ptr<AstNode> Parser::parseIfCond() {
    tokenWindow.advancePunctuator(PunctuatorType::OpenParenthesis);

    std::shared_ptr<AstNode> ifCond = parseExpression();

    tokenWindow.advancePunctuator(PunctuatorType::CloseParenthesis);

    return ifCond;
}