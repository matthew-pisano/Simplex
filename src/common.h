//
// Created by matthew on 8/29/23.
//

#ifndef SIMPLEX_COMMON_H
#define SIMPLEX_COMMON_H


#include "utils.h"

using namespace utils;


/**
 * The type codes that tokens can take on
 */
enum class TokenType {
    empty,
    link,
    op,
    assign,
    eos,
    sep,
    kwd,
    id,
    litInt,
    litFlt,
    litBln,
    litStr,
    bgnBlk,
    endBlk,
    bgnBkt,
    endBkt,
    bgnPar,
    endPar,
    prim,
};


/**
 * A single token of the program
 */
struct Token {
    /** The type id of the token */
    TokenType type;
    /** The raw value of the token */
    string value;

    Token();
    Token(TokenType type, const string& value);
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
    ne,
    range
};


#endif //SIMPLEX_COMMON_H
