#ifndef SIMPLE_PARSER_H
#define SIMPLE_PARSER_H

#include <vector>
#include <memory>

#include "../utils/token_sliding_window.h"
#include "../../backend/program.h"
#include "./types/type_factory.h"

#include "../lexer/tokens/token.h"

#include "./ast/ast_node.h"
#include "./ast/ast_function.h"
#include "./ast/ast_function_params.h"


class Parser {
public:
    Parser(const TokenSlidingWindow& tokenWindow): tokenWindow(tokenWindow) {}

    Program parse();

private:
    TokenSlidingWindow tokenWindow;
    TypeFactory typeFactory;

    int getPrecedents(const std::shared_ptr<Token>& token);

    std::shared_ptr<AstFunction> parseFunction();
    std::vector<AstFunctionParam> parseFunctionParams();
    std::vector<std::shared_ptr<AstNode>> parseBody();
    std::shared_ptr<AstNode> parseVarAssignmentOrFuncCall();
    std::shared_ptr<AstNode> parseVarDeclaration();
    std::shared_ptr<AstNode> parseExpression();
    std::shared_ptr<AstNode> parsePrimaryExpression();
    std::shared_ptr<AstNode> parseFuncCallOrVariable();
    std::shared_ptr<AstNode> parseNumberLiteral();
    std::shared_ptr<AstNode> parseBinaryOpExpression(std::shared_ptr<AstNode> lhs, int precedents);
    std::shared_ptr<AstNode> parseParenthesesExpression();
    std::shared_ptr<AstNode> parseFunctionCall(const std::string& identifier);
    std::shared_ptr<AstNode> parseVariable(const std::string& identifier);
    std::shared_ptr<AstNode> parseReturn();
    std::shared_ptr<AstNode> parseIfElse();
    std::shared_ptr<AstNode> parseIfCond();
};


#endif //SIMPLE_PARSER_H
