#ifndef SIMPLE_TYPE_I32_H
#define SIMPLE_TYPE_I32_H

#include "type.h"
#include "type_kind.h"

#include <llvm/IR/Type.h>

class CodeGenerator;

class TypeInt32: public Type {
public:
    TypeInt32(): Type(TypeKind::I32) {}

    virtual llvm::Type* codeGen(CodeGenerator& codeGenerator) override;
};

#endif //SIMPLE_TYPE_I32_H
