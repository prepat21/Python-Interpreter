#pragma once

#include <vector>
#include <string>

enum TokenType {
    // ids
    IDENTIFIER,

    // keywords
    IF, ELSE, DEF, RETURN, NOT, AND, OR, TRUE, FALSE, NONE, PRINT,

    // operators
    PLUS, MINUS, DIVIDE, MULTIPLY, EQUAL,
    EQUAL_TO, GREATER_THAN, LESS_THAN, NOT_EQUAL_TO,
    GREATER_THAN_EQUAL_TO, LESS_THAN_EQUAL_TO,

    // values
    NUMBER, STRING, LPARAN, RPARAN,

    // syntax
    NEWLINE, INDENT, DEDENT, COLON, END, COMMA,

    // invalid token
    INVALID,
};

std::vector<std::string> tokenNames = {
    // ids
    "IDENTIFIER",

    // keywords
    "IF", "ELSE", "DEF", "RETURN", "NOT", "AND", "OR", "TRUE", "FALSE", "NONE", "PRINT",

    // operators
    "PLUS", "MINUS", "DIVIDE", "MULTIPLY", "EQUAL",
    "EQUAL_TO", "GREATER_THAN", "LESS_THAN", "NOT_EQUAL_TO",
    "GREATER_THAN_EQUAL_TO", "LESS_THAN_EQUAL_TO",

    // values
    "NUMBER", "STRING", "LPARAN", "RPARAN",

    // syntax
    "NEWLINE", "INDENT", "DEDENT", "COLON", "END", "COMMA",

    // invalid token
    "INVALID",
};


struct Token {
    TokenType type;
    std::string value;

    Token() {}

    Token(TokenType t, std::string v) {
        type = t;
        value = v;
    }

    Token(TokenType t, char v) {
        type = t;
        value = v;
    }
};