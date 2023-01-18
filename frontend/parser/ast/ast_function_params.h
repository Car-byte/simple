#ifndef SIMPLE_AST_FUNCTION_PARAMS_H
#define SIMPLE_AST_FUNCTION_PARAMS_H

#include <string>
#include <memory>

#include "../types/type.h"

class AstFunctionParam {
public:
    AstFunctionParam(const std::string& identifier, std::shared_ptr<Type> paramType)
        : identifier(identifier), paramType(std::move(paramType)) {}

    std::string getIdentifier() {
        return identifier;
    }

    std::shared_ptr<Type> getParamType() {
        return paramType;
    }

private:
    std::string identifier;
    std::shared_ptr<Type> paramType;
};

#endif //SIMPLE_AST_FUNCTION_PARAMS_H
