#ifndef SIMPLE_TYPE_H
#define SIMPLE_TYPE_H

#include <llvm/IR/Type.h>
#include "type_kind.h"

class CodeGenerator;

class Type {
public:
    Type(TypeKind typeKind): typeKind(typeKind) {}
    virtual ~Type() = default;

    virtual bool isVoidType() const {
        return false;
    }

    virtual llvm::Type* codeGen(CodeGenerator& codeGenerator) = 0;

private:
    TypeKind typeKind;
};

#endif //SIMPLE_TYPE_H
