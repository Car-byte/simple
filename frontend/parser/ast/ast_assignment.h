#ifndef SIMPLE_AST_ASSIGNMENT_H
#define SIMPLE_AST_ASSIGNMENT_H

#include <memory>
#include <string>

#include "ast_node.h"

#include <llvm/IR/Value.h>

class CodeGenerator;

class AstAssignment: public AstNode {
public:
    AstAssignment(const std::string& identifier, std::shared_ptr<AstNode> assignedVal)
        : identifier(identifier), assignedVal(std::move(assignedVal)) {}

    virtual ~AstAssignment() override = default;

    virtual llvm::Value* codeGen(CodeGenerator& codeGenerator) override;

    std::string getIdentifier() const {
        return identifier;
    }

    std::shared_ptr<AstNode> getAssignedVal() {
        return assignedVal;
    }

private:
    std::string identifier;
    std::shared_ptr<AstNode> assignedVal;
};

#endif //SIMPLE_AST_ASSIGNMENT_H
