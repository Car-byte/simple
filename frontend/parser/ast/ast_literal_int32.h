#ifndef SIMPLE_AST_LITERAL_INT32_H
#define SIMPLE_AST_LITERAL_INT32_H

#include <string>

#include "ast_node.h"

#include <llvm/IR/Value.h>

class CodeGenerator;

class AstLiteralInt32: public AstNode {
public:
    AstLiteralInt32(const std::string& valStr): val(std::stoi(valStr)) {}

    int getVal() const {
        return val;
    }

    virtual ~AstLiteralInt32() override = default;

    virtual llvm::Value* codeGen(CodeGenerator& codeGenerator) override;

private:
    int val;
};

#endif //SIMPLE_AST_LITERAL_INT32_H
