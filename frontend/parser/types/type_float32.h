#ifndef SIMPLE_TYPE_FLOAT32_H
#define SIMPLE_TYPE_FLOAT32_H

#include "type.h"
#include "type_kind.h"

#include <llvm/IR/Type.h>

class CodeGenerator;

class TypeFloat32: public Type {
public:
    TypeFloat32(): Type(TypeKind::F32) {}

    virtual llvm::Type* codeGen(CodeGenerator& codeGenerator) override;
};

#endif //SIMPLE_TYPE_FLOAT32_H
