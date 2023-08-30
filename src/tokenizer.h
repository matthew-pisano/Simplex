//
// Created by matthew on 8/26/23.
//

#ifndef SIMPLEX_TOKENIZER_H
#define SIMPLEX_TOKENIZER_H

#include <utility>

#include <unordered_set>

#include "utils.h"
#include "common.h"


using namespace utils;


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
    static const std::map<string, OpCode> operators;
    /** The set of reserved language keywords */
    static const std::unordered_set<string> keywords;

    static const std::unordered_set<string> primitives;

    /** The end of statement token */
    static const string eos;

    static const string sep;

    static const string assign;

    vector<Token> tokens;

    /** The current, unprocessed portion of an identifier */
    string identStr;

    vector<Token> tokenize(string raw);

    Tokenizer() = default;

private:
    /**
     * Turns the raw ling of code into a vector of tokens
     * @param rawLine The line of code
     */
    void tokenizeLine(const string& rawLine);

    // static vector<Token> postProcessLine(const vector<Token>& tokens);

    // static bool matchPattern(TokenType pattern[], const vector<Token>& tokens);

    /**
     * Pushes a symbol to the token vector if the next one or two characters are a valid symbol
     * @param strAt The string character to process
     * @param strNext The string character that will come next
     * @return The number of characters that were found.  Zero, if none
     */
    int pushSymbol(const string& strAt, const Token& tokenPrev, const string& strNext);

    /**
     * Clears the partial identifier and pushes it to the token vector
     * @param identStr The current partial identifier
     * @param pushTo The vector to push to
     * @return Whether there was an identifier to push or if it was empty
     */
    Token processIdentifier();
};


string dumpTokens(const vector<Token>& tokens);

#endif //SIMPLEX_TOKENIZER_H
