#ifndef SIMPLE_TYPE_FACTORY_H
#define SIMPLE_TYPE_FACTORY_H

#include <memory>
#include <string>
#include <unordered_map>

#include "type.h"
#include "type_int32.h"
#include "type_float32.h"
#include "type_void.h"

class TypeFactory {
public:
    std::shared_ptr<Type> getType(const std::string& typeStr) {
        if (typeMap.contains(typeStr)) {
            return typeMap.at(typeStr);
        }

        // handle structs
        assert(false && "unsupported type in type factory");
    }

private:
    static inline const std::unordered_map<std::string, std::shared_ptr<Type>> typeMap = {
            {"i32", std::make_shared<TypeInt32>()},
            {"f32", std::make_shared<TypeFloat32>()},
            {"void", std::make_shared<TypeVoid>()},
    };
};

#endif //SIMPLE_TYPE_FACTORY_H
