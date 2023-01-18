#ifndef SIMPLE_AST_FUNCTION_RETURN_H
#define SIMPLE_AST_FUNCTION_RETURN_H

#include <memory>

#include "ast_node.h"

#include <llvm/IR/Value.h>

class CodeGenerator;

class AstFunctionReturn: public AstNode {
public:
    AstFunctionReturn(std::shared_ptr<AstNode> returnVal): returnVal(std::move(returnVal)) {}

    virtual ~AstFunctionReturn() override = default;

    virtual llvm::Value* codeGen(CodeGenerator& codeGenerator) override;

    std::shared_ptr<AstNode> getReturnVal() {
        return returnVal;
    }

private:
    std::shared_ptr<AstNode> returnVal;
};

#endif //SIMPLE_AST_FUNCTION_RETURN_H
