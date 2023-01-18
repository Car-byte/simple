#ifndef SIMPLE_AST_VARIABLE_H
#define SIMPLE_AST_VARIABLE_H

#include <string>

#include "ast_node.h"

#include <llvm/IR/Value.h>

class CodeGenerator;

class AstVariable: public AstNode {
public:
    AstVariable(const std::string& identifier): identifier(identifier) {}

    virtual ~AstVariable() override = default;

    virtual llvm::Value* codeGen(CodeGenerator& codeGenerator) override;

    std::string getIdentifier() const {
        return identifier;
    }

private:
    std::string identifier;
};

#endif //SIMPLE_AST_VARIABLE_H
