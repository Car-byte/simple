#ifndef SIMPLE_AST_VAR_DECLARATION_H
#define SIMPLE_AST_VAR_DECLARATION_H

#include <string>
#include <memory>

#include "ast_node.h"
#include "../types/type.h"

#include <llvm/IR/Value.h>

class CodeGenerator;

class AstVarDeclaration: public AstNode {
public:
    AstVarDeclaration(const std::string& identifier,
                      std::shared_ptr<Type> type,
                      std::shared_ptr<AstNode> assignedVal)
       : identifier(identifier), type(std::move(type)),
       assignedVal(std::move(assignedVal)) {}

    virtual ~AstVarDeclaration() override = default;

    virtual llvm::Value* codeGen(CodeGenerator& codeGenerator) override;

    std::string getIdentifier() const {
        return identifier;
    }

    std::shared_ptr<Type> getType() {
        return type;
    }

    std::shared_ptr<AstNode> getAssignedVal() {
        return assignedVal;
    }

private:
    std::string identifier;
    std::shared_ptr<Type> type;
    std::shared_ptr<AstNode> assignedVal;
};

#endif //SIMPLE_AST_VAR_DECLARATION_H
