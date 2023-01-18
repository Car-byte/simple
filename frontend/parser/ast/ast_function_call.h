#ifndef SIMPLE_AST_FUNCTION_CALL_H
#define SIMPLE_AST_FUNCTION_CALL_H

#include <vector>
#include <memory>

#include "ast_node.h"

#include <llvm/IR/Value.h>

class CodeGenerator;

class AstFunctionCall: public AstNode {
public:
    AstFunctionCall(const std::string& identifier, std::vector<std::shared_ptr<AstNode>>& args)
        : identifier(identifier), args(std::move(args)) {}

    virtual ~AstFunctionCall() override = default;

    virtual llvm::Value* codeGen(CodeGenerator& codeGenerator) override;

private:
    std::string identifier;
    std::vector<std::shared_ptr<AstNode>> args;
};

#endif //SIMPLE_AST_FUNCTION_CALL_H
