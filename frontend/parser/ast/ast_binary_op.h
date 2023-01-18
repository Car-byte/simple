#ifndef SIMPLE_AST_BINARY_OP_H
#define SIMPLE_AST_BINARY_OP_H

#include <string>
#include <vector>
#include <memory>

#include "ast_node.h"
#include "ast_function_params.h"
#include "../types/type.h"
#include "../../lexer/tokens/operator_type.h"

#include <llvm/IR/Value.h>

class CodeGenerator;

class AstBinaryOp: public AstNode {
public:
    AstBinaryOp(OperatorType operatorType,
                std::shared_ptr<AstNode> lhs,
                std::shared_ptr<AstNode> rhs)
        : operatorType(operatorType), lhs(std::move(lhs)), rhs(std::move(rhs)) {}

    virtual ~AstBinaryOp() override = default;

    std::shared_ptr<AstNode> getLhs() {
        return lhs;
    }

    std::shared_ptr<AstNode> getRhs() {
        return rhs;
    }

    OperatorType getOperatorType() const {
        return operatorType;
    }

    virtual llvm::Value* codeGen(CodeGenerator& codeGenerator) override;

private:
    OperatorType operatorType;
    std::shared_ptr<AstNode> lhs;
    std::shared_ptr<AstNode> rhs;
};

#endif //SIMPLE_AST_BINARY_OP_H
