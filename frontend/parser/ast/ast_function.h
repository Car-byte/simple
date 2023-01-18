#ifndef SIMPLE_AST_FUNCTION_H
#define SIMPLE_AST_FUNCTION_H

#include <string>
#include <vector>
#include <memory>

#include "ast_node.h"
#include "ast_function_params.h"
#include "../types/type.h"

#include <llvm/IR/Value.h>

class CodeGenerator;

class AstFunction: public AstNode {
public:
    AstFunction(const std::string& identifier,
                    const std::vector<AstFunctionParam>& funcParams,
                    std::vector<std::shared_ptr<AstNode>>& body,
                    std::shared_ptr<Type> returnType)
        : identifier(identifier), funcParams(funcParams),
        body(std::move(body)), returnType(std::move(returnType)) {}

    virtual ~AstFunction() override = default;

    std::string getIdentifier() {
        return identifier;
    }

    std::vector<AstFunctionParam> getFuncParams() {
        return funcParams;
    }

    std::vector<std::shared_ptr<AstNode>> getBody() {
        return body;
    }

    std::shared_ptr<Type> getReturnType() {
        return returnType;
    }

    virtual llvm::Value* codeGen(CodeGenerator& codeGenerator) override;

private:
    std::string identifier;
    std::vector<AstFunctionParam> funcParams;
    std::vector<std::shared_ptr<AstNode>> body;
    std::shared_ptr<Type> returnType;
};

#endif //SIMPLE_AST_FUNCTION_H
