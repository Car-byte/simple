#ifndef SIMPLE_AST_LITERAL_FLOAT32_H
#define SIMPLE_AST_LITERAL_FLOAT32_H

#include <string>

#include "ast_node.h"

#include <llvm/IR/Value.h>

class CodeGenerator;

class AstLiteralFloat32: public AstNode {
public:
    AstLiteralFloat32(const std::string& valStr): val(std::stof(valStr)) {}

    float getVal() const {
        return val;
    }

    virtual ~AstLiteralFloat32() override = default;

    virtual llvm::Value* codeGen(CodeGenerator& codeGenerator) override;

private:
    float val;
};

#endif //SIMPLE_AST_LITERAL_FLOAT32_H
