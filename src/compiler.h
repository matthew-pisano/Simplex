//
// Created by matthew on 8/26/23.
//

#ifndef SIMPLEX_COMPILER_H
#define SIMPLEX_COMPILER_H

#include <functional>
#include <unordered_set>

#include "utils.h"
#include "tokenizer.h"


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


enum class OpCode {
    mkVar,
    rmVar,
    mkFnc,
    rmFnc,
    setVar,
    getVar,
    callFnc,
    add,
    sub,
    mul,
    div,
    binAnd,
    binOr,
    unNot,
    gt,
    lt,
    ge,
    le,
    eq,
    ne
};


struct Instruction {
    OpCode opCode;
    vector<string> args;

    Instruction(OpCode opCode, vector<string> args);
};

class Compiler {
    static std::map<string, Operator*> opMap;

    static vector<Instruction> compile(const vector<Token>& tokens);

private:
    static vector<Instruction> compileLine(vector<Token> tokens, map<string, int>& functionGoTos);

    static int getClosingIndex(vector<Token> tokens, int start, string openBlock, string closeBlock);

    static vector<Instruction> compileExpression(vector<Token> tokens, int start, int end);

    static OpCode getLineOpCode(const vector<Token>& tokens);
};


#endif //SIMPLEX_COMPILER_H
