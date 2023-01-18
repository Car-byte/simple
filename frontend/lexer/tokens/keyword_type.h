#ifndef SIMPLE_KEYWORD_TYPE_H
#define SIMPLE_KEYWORD_TYPE_H

#include <string>
#include <unordered_map>

enum class KeywordType {
    Func,
    Var,
};

static const std::unordered_map<std::string, KeywordType> keyWordMap = {
        {"func", KeywordType::Func},
        {"var", KeywordType::Var},
};

#endif //SIMPLE_KEYWORD_TYPE_H
