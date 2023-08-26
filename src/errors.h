//
// Created by matthew on 8/26/23.
//

#ifndef SIMPLEX_ERRORS_H
#define SIMPLEX_ERRORS_H


#include <exception>

#include "utils.h"

using namespace utils;


/**
 * A Simplex syntax error
 */
class SyntaxError : public std::exception {
    char* message;

public:
    explicit SyntaxError(const string& msg);

    char* what();
};


#endif //SIMPLEX_ERRORS_H
