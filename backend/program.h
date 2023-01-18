#ifndef SIMPLE_PROGRAM_H
#define SIMPLE_PROGRAM_H

#include <vector>
#include <memory>

#include "../frontend/parser/ast/ast_function.h"

class Program {
public:
    Program(std::vector<std::shared_ptr<AstFunction>>& functions,
            std::shared_ptr<AstFunction>& mainFunc)
       : functions(std::move(functions)), mainFunc(std::move(mainFunc)) {}

    std::vector<std::shared_ptr<AstFunction>> getFunctions() {
        return functions;
    }

    std::shared_ptr<AstFunction> getMainFunc() {
        return mainFunc;
    }

    bool shouldRunOptimizations() {
        return shouldOptimize;
    }

    void setShouldRunOptimizations(bool shouldOptimize) {
        this->shouldOptimize = shouldOptimize;
    }

private:
    std::vector<std::shared_ptr<AstFunction>> functions;
    std::shared_ptr<AstFunction> mainFunc;
    bool shouldOptimize = false;
};

#endif //SIMPLE_PROGRAM_H
