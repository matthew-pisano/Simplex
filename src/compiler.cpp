//
// Created by matthew on 8/26/23.
//

#include <utility>

#include "compiler.h"


Operator::Operator(string token) {
    this->token = std::move(token);
}

Type::Type(string& token, map<Operator, std::function<any(any, any)>> opMap) {
    this->token = token;
    this->opDefs = std::move(opMap);
}

const std::unordered_set<string> Compiler::primitives = {"int", "bool", "float", "void", "str", "any"};