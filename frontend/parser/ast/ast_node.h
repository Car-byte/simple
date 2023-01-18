#ifndef SIMPLE_AST_NODE_H
#define SIMPLE_AST_NODE_H

#include <llvm/IR/Value.h>

class CodeGenerator;

class AstNode {
public:
    virtual ~AstNode() = default;
    virtual llvm::Value* codeGen(CodeGenerator& codeGenerator) = 0;
};

#endif //SIMPLE_AST_NODE_H
