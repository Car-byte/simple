#ifndef SIMPLE_TYPE_VOID_H
#define SIMPLE_TYPE_VOID_H

#include "type.h"
#include "type_kind.h"

#include <llvm/IR/Type.h>

class CodeGenerator;

class TypeVoid: public Type {
public:
    TypeVoid(): Type(TypeKind::Void) {}

    virtual llvm::Type* codeGen(CodeGenerator& codeGenerator) override;

    virtual bool isVoidType() const override {
        return true;
    }
};

#endif //SIMPLE_TYPE_VOID_H
