#include "code_generator.h"

#include <vector>
#include <memory>

#include "program.h"

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

#include <llvm/IR/Value.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/Host.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/LegacyPassManagers.h>
#include <llvm/Transforms/Utils.h>
#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Scalar/GVN.h>


void CodeGenerator::codeGenProgram() {
    assert(program.getMainFunc());
    setBuildInfo();

    codeGenFuncPrototypes();
    codeGenFuncs();
    codeGenMainFunc();

    if (program.shouldRunOptimizations()) {
        runOptimizations();
    }
}

void CodeGenerator::codeGenFuncPrototypes() {
    for (const auto& funcPtr: program.getFunctions()) {
        llvm::FunctionType* funcType = getFuncType(funcPtr);
        llvm::Function::Create(funcType, llvm::Function::PrivateLinkage,
                               funcPtr->getIdentifier(), module);
    }
}

void CodeGenerator::codeGenFuncs() {
    for (const auto& funcPtr: program.getFunctions()) {
        codeGenFunc(funcPtr);
    }
}

void CodeGenerator::codeGenFunc(const std::shared_ptr<AstFunction>& funcPtr) {
    llvm::Function* func = module.getFunction(funcPtr->getIdentifier());
    llvm::BasicBlock* entryBasicBlock = llvm::BasicBlock::Create(context, "entry", func);
    builder.SetInsertPoint(entryBasicBlock);
    vars.clear();

    for (auto& param : func->args()) {
        unsigned int paramIdx = param.getArgNo();
        std::string paramName = funcPtr->getFuncParams()[paramIdx].getIdentifier();
        llvm::Type* paramType = func->getFunctionType()->getParamType(paramIdx);

        vars[paramName] = builder.CreateAlloca(paramType, nullptr, paramName);
        builder.CreateStore(&param, vars[paramName]);
    }

    llvm::Value* returnedValue = nullptr;
    for (const auto &node: funcPtr->getBody()) {
        returnedValue = node->codeGen(*this);
    }

    if (func->getReturnType()->isVoidTy()) {
        builder.CreateRetVoid();
    }

    bool isError = llvm::verifyFunction(*func, &llvm::outs());
    assert(!isError);
}

void CodeGenerator::codeGenMainFunc() {
    llvm::FunctionType* mainType = llvm::FunctionType::get(llvm::IntegerType::getInt32Ty(context),
                                                           std::vector<llvm::Type*>(), false);
    llvm::Function* main = llvm::Function::Create(mainType, llvm::Function::ExternalLinkage,
    "main", module);
    llvm::BasicBlock* mainBasicBlock = llvm::BasicBlock::Create(context, "entry", main);
    builder.SetInsertPoint(mainBasicBlock);

    vars.clear();
    for (const auto& node: program.getMainFunc()->getBody()) {
        node->codeGen(*this);
    }

    llvm::APInt retVal(32, 0, true);
    builder.CreateRet(llvm::ConstantInt::get(context, retVal));

    bool isError = llvm::verifyFunction(*main, &llvm::outs());
    assert(!isError);
}

void CodeGenerator::runOptimizations() {
    llvm::legacy::FunctionPassManager functionPassManager(&module);

    functionPassManager.add(llvm::createPromoteMemoryToRegisterPass());
    functionPassManager.add(llvm::createInstructionCombiningPass());
    functionPassManager.add(llvm::createReassociatePass());
    functionPassManager.add(llvm::createGVNPass());
    functionPassManager.add(llvm::createCFGSimplificationPass());
    functionPassManager.add(llvm::createDeadCodeEliminationPass());
    functionPassManager.add(llvm::createDeadStoreEliminationPass());

    functionPassManager.doInitialization();

    for (const auto& funcPtr: program.getFunctions()) {
        llvm::Function* func = module.getFunction(funcPtr->getIdentifier());
        functionPassManager.run(*func);
    }

    llvm::Function* mainFunc = module.getFunction(program.getMainFunc()->getIdentifier());
    functionPassManager.run(*mainFunc);
}

llvm::FunctionType* CodeGenerator::getFuncType(const std::shared_ptr<AstFunction>& func) {
    std::vector<llvm::Type*> paramTypes;

    for (auto& param: func->getFuncParams()) {
        paramTypes.push_back(param.getParamType()->codeGen(*this));
    }

    llvm::Type* returnType = func->getReturnType()->codeGen(*this);

    return llvm::FunctionType::get(returnType, paramTypes, false);
}

void CodeGenerator::setBuildInfo() {
    std::string targetTriple = llvm::sys::getDefaultTargetTriple();
    module.setTargetTriple(targetTriple);
}

llvm::Value* CodeGenerator::codeGen(AstAssignment& node) {
    llvm::Value* assignedVal = node.getAssignedVal()->codeGen(*this);
    llvm::AllocaInst* var = vars[node.getIdentifier()];

    builder.CreateStore(assignedVal, var);
    return assignedVal;
}

llvm::Value* CodeGenerator::codeGen(AstBinaryOp& node) {
    llvm::Value* lhs = node.getLhs()->codeGen(*this);
    llvm::Value* rhs = node.getRhs()->codeGen(*this);

    assert(lhs && rhs);

    switch (node.getOperatorType()) {
        case OperatorType::Add:
            return builder.CreateAdd(lhs, rhs);
        case OperatorType::Sub:
            return builder.CreateSub(lhs, rhs);
        case OperatorType::LogicalEqual:
            return builder.CreateICmpEQ(lhs, rhs);
        case OperatorType::NotLogicalEqual:
            return builder.CreateICmpNE(lhs, rhs);
        case OperatorType::Lesser:
            return builder.CreateICmpSLT(lhs, rhs);
        case OperatorType::LesserEqual:
            return builder.CreateICmpSLE(lhs, rhs);
        case OperatorType::Greater:
            return builder.CreateICmpSGT(lhs, rhs);
        case OperatorType::GreaterEqual:
            return builder.CreateICmpSGE(lhs, rhs);
        default:
            assert(false && "unsupported binary op");
    }
}

llvm::Value* CodeGenerator::codeGen(AstFunctionCall& node) {
    llvm::Function* func = module.getFunction(node.getIdentifier());
    assert(func && "unknown function");
    llvm::FunctionType* funcType = func->getFunctionType();
    assert(node.getArgs().size() == funcType->getNumParams());
    std::vector<llvm::Value*> args;

    for (int i = 0; i < node.getArgs().size(); i++) {
        llvm::Value* arg = node.getArgs()[i]->codeGen(*this);
        llvm::Type* paramType = funcType->getParamType(i);
        llvm::Value* bitCastArgVal = builder.CreateBitCast(arg, paramType);
        args.push_back(bitCastArgVal);
    }

    return builder.CreateCall(func, args);
}

llvm::Value* CodeGenerator::codeGen(AstFunction& node) {
    assert(false && "unimplemented function node");
}

llvm::Value* CodeGenerator::codeGen(AstLiteralInt32& node) {
    return llvm::ConstantInt::getSigned(llvm::Type::getInt32Ty(context), node.getVal());
}

llvm::Value* CodeGenerator::codeGen(AstLiteralFloat32& node) {
    return llvm::ConstantFP::get(llvm::Type::getFloatTy(context), node.getVal());
}

llvm::Value* CodeGenerator::codeGen(AstVarDeclaration& node) {
    llvm::Value* val = node.getAssignedVal()->codeGen(*this);
    llvm::Function* parentFunction = builder.GetInsertBlock()->getParent();

    llvm::IRBuilder<> TmpBuilder(&(parentFunction->getEntryBlock()),
                                 parentFunction->getEntryBlock().begin());
    llvm::AllocaInst* var = TmpBuilder.CreateAlloca(val->getType(), nullptr,
                                                    node.getIdentifier());

    vars[node.getIdentifier()] = var;
    builder.CreateStore(val, var);

    return val;
}

llvm::Value* CodeGenerator::codeGen(AstVariable& node) {
    llvm::AllocaInst* var = vars[node.getIdentifier()];
    assert(var && "undeclared variable referenced");
    return builder.CreateLoad(var->getAllocatedType(), var);
}

llvm::Value* CodeGenerator::codeGen(AstFunctionReturn& node) {
    llvm::Value* returnVal = node.getReturnVal()->codeGen(*this);
    builder.CreateRet(returnVal);
    return returnVal;
}

llvm::Value* CodeGenerator::codeGen(AstIfElse& ifElseNode) {
    llvm::Value* condVal = ifElseNode.getIfCond()->codeGen(*this);
    llvm::Function* parentFunc = builder.GetInsertBlock()->getParent();

    llvm::BasicBlock* ifBlock = llvm::BasicBlock::Create(context, "ifBlock", parentFunc);
    llvm::BasicBlock* elseBlock = llvm::BasicBlock::Create(context, "elseBlock");
    llvm::BasicBlock* mergeBlock = llvm::BasicBlock::Create(context, "ifcont");

    builder.CreateCondBr(condVal, ifBlock, elseBlock);
    builder.SetInsertPoint(ifBlock);

    llvm::Value* ifVal = nullptr;
    for (const auto& node: ifElseNode.getIfBody()) {
        ifVal = node->codeGen(*this);
    }

    assert(ifVal);

    ifBlock = builder.GetInsertBlock();
    builder.CreateBr(mergeBlock);

    parentFunc->getBasicBlockList().push_back(elseBlock);
    builder.SetInsertPoint(elseBlock);

    llvm::Value* elseVal = nullptr;
    for (const auto& node: ifElseNode.getElseBody()) {
        elseVal = node->codeGen(*this);
    }
    assert(elseVal);

    elseBlock = builder.GetInsertBlock();
    builder.CreateBr(mergeBlock);

    parentFunc->getBasicBlockList().push_back(mergeBlock);
    builder.SetInsertPoint(mergeBlock);

    if (ifVal->getType() == llvm::Type::getVoidTy(context) ||
        elseVal->getType() == llvm::Type::getVoidTy(context) ||
        (ifVal->getType() != elseVal->getType())) {
        return llvm::Constant::getNullValue(llvm::Type::getInt32Ty(context));
    }

    llvm::PHINode* phiNode = builder.CreatePHI(ifVal->getType(), 2, "iftmp");
    phiNode->addIncoming(ifVal, ifBlock);
    phiNode->addIncoming(elseVal, elseBlock);

    return phiNode;
}

llvm::Type* CodeGenerator::codeGen(TypeInt32& type) {
    return llvm::Type::getInt32Ty(context);
}

llvm::Type* CodeGenerator::codeGen(TypeFloat32& type) {
    return llvm::Type::getFloatTy(context);
}

llvm::Type* CodeGenerator::codeGen(TypeVoid& type) {
    return llvm::Type::getVoidTy(context);
}