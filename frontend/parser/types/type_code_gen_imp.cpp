#include "type.h"
#include "type_float32.h"
#include "type_void.h"
#include "type_int32.h"

#include "../../../backend/code_generator.h"

llvm::Type* TypeFloat32::codeGen(CodeGenerator& codeGenerator) {
    return codeGenerator.codeGen(*this);
}

llvm::Type* TypeInt32::codeGen(CodeGenerator& codeGenerator) {
    return codeGenerator.codeGen(*this);
}

llvm::Type* TypeVoid::codeGen(CodeGenerator& codeGenerator) {
    return codeGenerator.codeGen(*this);
}
