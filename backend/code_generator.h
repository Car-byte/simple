#ifndef SIMPLE_CODE_GENERATOR_H
#define SIMPLE_CODE_GENERATOR_H

#include <unordered_map>
#include <string>
#include <memory>

#include "../frontend/parser/ast/ast_node.h"
#include "../frontend/parser/ast/ast_assignment.h"
#include "../frontend/parser/ast/ast_binary_op.h"
#include "../frontend/parser/ast/ast_function.h"
#include "../frontend/parser/ast/ast_function_call.h"
#include "../frontend/parser/ast/ast_literal_float32.h"
#include "../frontend/parser/ast/ast_literal_int32.h"
#include "../frontend/parser/ast/ast_var_declaration.h"
#include "../frontend/parser/ast/ast_variable.h"
#include "../frontend/parser/ast/ast_function_return.h"
#include "../frontend/parser/ast/ast_if_else.h"

#include "../frontend/parser/types/type_void.h"
#include "../frontend/parser/types/type_float32.h"
#include "../frontend/parser/types/type_int32.h"

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Value.h>

#include "program.h"

class CodeGenerator {
public:
    CodeGenerator(const Program& program): program(program) {}

    void codeGenProgram();
    void dumpIr() {
        module.print(llvm::outs(), nullptr);
    }

    llvm::Value* codeGen(AstAssignment& node);
    llvm::Value* codeGen(AstBinaryOp& node);
    llvm::Value* codeGen(AstFunctionCall& node);
    llvm::Value* codeGen(AstFunction& node);
    llvm::Value* codeGen(AstLiteralInt32& node);
    llvm::Value* codeGen(AstLiteralFloat32& node);
    llvm::Value* codeGen(AstVarDeclaration& node);
    llvm::Value* codeGen(AstVariable& node);
    llvm::Value* codeGen(AstFunctionReturn& node);
    llvm::Value* codeGen(AstIfElse& node);

    llvm::Type* codeGen(TypeInt32& type);
    llvm::Type* codeGen(TypeFloat32& type);
    llvm::Type* codeGen(TypeVoid& type);

private:
    llvm::LLVMContext context;
    llvm::IRBuilder<> builder = llvm::IRBuilder<>(context);
    llvm::Module module = llvm::Module("simple", context);

    Program program;
    std::unordered_map<std::string, llvm::AllocaInst*> vars;

    void setBuildInfo();
    void codeGenFuncPrototypes();
    void codeGenFuncs();
    void codeGenFunc(const std::shared_ptr<AstFunction>& func);
    void codeGenMainFunc();
    void runOptimizations();
    llvm::FunctionType* getFuncType(const std::shared_ptr<AstFunction>& func);
};


#endif //SIMPLE_CODE_GENERATOR_H
