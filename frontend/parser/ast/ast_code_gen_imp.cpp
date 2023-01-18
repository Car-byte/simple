#include "ast_node.h"
#include "ast_literal_float32.h"
#include "ast_function.h"
#include "ast_var_declaration.h"
#include "ast_literal_int32.h"
#include "ast_function_call.h"
#include "ast_binary_op.h"
#include "ast_assignment.h"
#include "ast_variable.h"
#include "ast_function_return.h"
#include "ast_if_else.h"

#include "../../../backend/code_generator.h"


llvm::Value* AstAssignment::codeGen(CodeGenerator& codeGenerator) {
    return codeGenerator.codeGen(*this);
}

llvm::Value* AstBinaryOp::codeGen(CodeGenerator& codeGenerator) {
    return codeGenerator.codeGen(*this);
}

llvm::Value* AstFunction::codeGen(CodeGenerator& codeGenerator) {
    return codeGenerator.codeGen(*this);
}

llvm::Value* AstFunctionCall::codeGen(CodeGenerator& codeGenerator) {
    return codeGenerator.codeGen(*this);
}

llvm::Value* AstLiteralFloat32::codeGen(CodeGenerator& codeGenerator) {
    return codeGenerator.codeGen(*this);
}

llvm::Value* AstLiteralInt32::codeGen(CodeGenerator& codeGenerator) {
    return codeGenerator.codeGen(*this);
}

llvm::Value* AstVarDeclaration::codeGen(CodeGenerator& codeGenerator) {
    return codeGenerator.codeGen(*this);
}

llvm::Value* AstVariable::codeGen(CodeGenerator& codeGenerator) {
    return codeGenerator.codeGen(*this);
}

llvm::Value* AstFunctionReturn::codeGen(CodeGenerator& codeGenerator) {
    return codeGenerator.codeGen(*this);
}

llvm::Value* AstIfElse::codeGen(CodeGenerator& codeGenerator) {
    return codeGenerator.codeGen(*this);
}
