//
// Created by matthew on 8/26/23.
//

#include "errors.h"


SyntaxError::SyntaxError(const string& msg) : message(const_cast<char*>(msg.c_str())) {}

char* SyntaxError::what() { return this->message; }
