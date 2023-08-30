//
// Created by matthew on 8/29/23.
//

#include "common.h"


Token::Token() {
    type = TokenType::empty;
    value = "";
}

Token::Token(TokenType type, const string& value) {
    this->type = type;
    this->value = value;
}
