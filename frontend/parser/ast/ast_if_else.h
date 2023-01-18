#ifndef SIMPLE_AST_IF_ELSE_H
#define SIMPLE_AST_IF_ELSE_H

#include <memory>
#include <vector>

#include "ast_node.h"

class CodeGenerator;

class AstIfElse: public AstNode {
public:
    AstIfElse(std::shared_ptr<AstNode> ifCond,
              const std::vector<std::shared_ptr<AstNode>>& ifBody,
              const std::vector<std::shared_ptr<AstNode>>& elseBody)
        : ifCond(std::move(ifCond)), ifBody(ifBody), elseBody(elseBody) {}

    virtual ~AstIfElse() override = default;

    virtual llvm::Value* codeGen(CodeGenerator& codeGenerator) override;

    std::shared_ptr<AstNode> getIfCond() {
        return ifCond;
    }

    std::vector<std::shared_ptr<AstNode>> getIfBody() {
        return ifBody;
    }

    std::vector<std::shared_ptr<AstNode>> getElseBody() {
        return elseBody;
    }

private:
    std::shared_ptr<AstNode> ifCond;
    std::vector<std::shared_ptr<AstNode>> ifBody;
    std::vector<std::shared_ptr<AstNode>> elseBody;
};

#endif //SIMPLE_AST_IF_ELSE_H
