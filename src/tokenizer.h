//
// Created by matthew on 8/26/23.
//

#ifndef SIMPLEX_TOKENIZER_H
#define SIMPLEX_TOKENIZER_H

#include <utility>

#include <unordered_set>

#include "utils.h"


using namespace utils;


/**
 * The type codes that tokens can take on
 */
enum class TokenTypes {
    op = 0,
    eos = 1,
    sep = 2,
    kwd = 3,
    id = 4,
    litInt = 5,
    litFlt = 6,
    litBln = 7,
    litStr = 8,
    bgnBlk = 9,
    endBlk = 10,
    bgnBkt = 11,
    endBkt = 12,
    bgnPar = 13,
    endPar = 14
};


/**
 * A single token of the program
 */
struct Token {
    /** The type id of the token */
    TokenTypes type;
    /** The raw value of the token */
    string value;

    Token(TokenTypes type, const string& value);
};


/**
 * The tokenizer for processing raw code
 */
struct Tokenizer {

    /** The set of string delimiters */
    static const std::unordered_set<string> stringDelim;
    /** The set of code block delimiters */
    static const std::unordered_set<string> blockDelim;
    /** The set of paren block delimiters */
    static const std::unordered_set<string> parenDelim;
    /** The set of bracket block delimiters */
    static const std::unordered_set<string> bracketDelim;
    /** The set of valid operators */
    static const std::unordered_set<string> operators;
    /** The set of reserved language keywords */
    static const std::unordered_set<string> keywords;
    /** The list of token separators */
    static const std::unordered_set<string> separators;

    /** The end of statement token */
    static const string eos;

    static vector<Token> tokenize(string raw);

private:
    /**
     * Turns the raw ling of code into a vector of tokens
     * @param rawLine The line of code
     * @return The tokenized line as a vector
     */
    static vector<Token> tokenizeLine(const string& rawLine);

    /**
     * Pushes a symbol to the token vector if the next one or two characters are a valid symbol
     * @param strAt The string character to process
     * @param strNext The string character that will come next
     * @param identStr The last non-symbol characters, a partial identifier
     * @param pushTo The vector to push the symbol token to
     * @return The number of characters that were found.  Zero, if none
     */
    static int pushSymbol(const string& strAt, const string& strNext, string& identStr, vector<Token>& pushTo);

    /**
     * Clears the partial identifier and pushes it to the token vector
     * @param identStr The current partial identifier
     * @param pushTo The vector to push to
     * @return Whether there was an identifier to push or if it was empty
     */
    static bool pushIdentifier(string& identStr, vector<Token>& pushTo);
};


string dumpTokens(const vector<Token>& tokens);

#endif //SIMPLEX_TOKENIZER_H
