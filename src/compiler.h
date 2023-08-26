//
// Created by matthew on 8/26/23.
//

#ifndef SIMPLEX_COMPILER_H
#define SIMPLEX_COMPILER_H

#include <functional>
#include <unordered_set>

#include "utils.h"


using namespace utils;

struct Operator {
    string token;

    explicit Operator(string token);
};


struct Type {
    string token;
    map <Operator, std::function<any(any, any)>> opDefs;

    Type(string& token, map<Operator, std::function<any(any, any)>> opMap);
};

class Compiler {
    static const std::unordered_set<string> primitives;
    static std::map<string, Operator*> opMap;
};


#endif //SIMPLEX_COMPILER_H
