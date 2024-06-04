#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <fstream>
#include <map>
#include "token.h"
#include <stdexcept>

#define INDENT_SIZE 4

class Scanner {
public:
    Scanner(std::string input) {
        code = input;
        scanTokens();
    };

    std::vector<Token> getTokens() {
        return tokens;
    }

    void printTokens() {
        for (auto t : tokens) {
            std::cout << "Token: " << tokenNames[t.type] << " Value: " << t.value << "\n";
        }
    }

private:
    std::string code;
    std::vector<Token> tokens;
    int start = 0;
    int current = 0;
    bool checkIndent = false;
    int currSpaces = 0;
    std::map<std::string, TokenType> keywords = {
        {"if", IF},
        {"else", ELSE},
        {"def", DEF},
        {"return", RETURN},
        {"not", NOT},
        {"and", AND},
        {"or", OR},
        {"True", TRUE},
        {"False", FALSE},
        {"None", NONE},
        {"print", PRINT}
    };

    void scanTokens() {
        while (!isAtEnd()) {
            start = current;
            scanToken();
        }
        addToken(END);
    }

    void scanToken() {
        char c = advance();
        if (c == '#') {
            while (peek() != '\n' && !isAtEnd()) {
                advance();
            }
            checkIndent = false;
            return;
        }
        if (c == '\r') {
            return;
        }
        if (c == '\t') {
            return;
        }
        if (c == '\n') {
            addToken(NEWLINE);
            checkIndent = true;
            return;
        }
        if (checkIndent) {
            if (c == ' ') {
                indent(1);
            }
            else {
                indent(0);
            }
        }
        if (c == '(') 
        {
            addToken(LPARAN); 
            return;
        }
        if (c == ')') 
        {
            addToken(RPARAN); 
            return;
        }
        if (c == '-') 
        {
            addToken(MINUS); 
            return;
        }
        if (c == '+') 
        {
            addToken(PLUS); 
            return;
        }
        if (c == '/') 
        {
            addToken(DIVIDE); 
            return;
        }
        if (c == '*') 
        {
            addToken(MULTIPLY); 
            return;
        }
        if (c == ',') {
            addToken(COMMA);
            return;
        }
        if (c == ':')
        {
            addToken(COLON); 
            return;
        }
        if (c == '!') 
        {
            match('=') ? addToken(NOT_EQUAL_TO) : error(); 
            return;
        }
        if (c == '=') 
        {
            addToken(match('=') ? EQUAL_TO : EQUAL); 
            return;
        }
        if (c == '<') 
        {
            addToken(match('=') ? LESS_THAN_EQUAL_TO : LESS_THAN); 
            return;
        }
        if (c == '>') 
        {
            addToken(match('=') ? GREATER_THAN_EQUAL_TO : GREATER_THAN); 
            return;
        }
        if (c == ' ') return;
        if (c == '"') 
        {
            string(); 
            return;
        }
        if (isNumeric(c)) 
        {
            number(); 
            return;
        }
        if (isAlpha(c)) 
        {
            identifier(); 
            return;
        }
        error();
        return;
    }

    char advance() {
        return code[current++];
    }

    bool match(char expected) {
        if (isAtEnd()) return false;
        if (code[current] != expected) return false;

        current++;
        return true;
    }

    char peek() {
        if (isAtEnd()) return '\0';
        return code[current];
    }

    char peekNext() {
        if (current + 1 >= code.size()) return '\0';
        return code[current + 1];
    }

    bool isAtEnd() {
        return (current >= code.size());
    }

    bool isAlpha(char c) {
        return std::isalpha(c);
    }

    bool isNumeric(char c) {
        return std::isdigit(c);
    }

    bool isAlphaNumeric(char c) {
        return isAlpha(c) || isNumeric(c);
    }

    bool isIdentifierCharacter(char c) {
        return isAlphaNumeric(c) || c == '_';
    }

    void indent(int spaces) {
        if (spaces == 1) {
            while (peek() == ' ' && !isAtEnd()) {
                advance();
                spaces++;
            }
        }

        if (peek() == '\n') {
            checkIndent = false;
            return;
        }
        
        //std::cout << spaces << " " << currSpaces << "\n";
        //std::cout << spaces % INDENT_SIZE << "\n";
        //std::cout << start << "\n";

        if ((spaces % INDENT_SIZE) != 0) {
            error();
        }

        int temp_spaces = spaces;
        while (currSpaces > temp_spaces) {
            addToken(DEDENT);
            temp_spaces += INDENT_SIZE;
        }
        while (currSpaces < temp_spaces) {
            addToken(INDENT);
            temp_spaces -= INDENT_SIZE;
        }

        currSpaces = spaces;
        checkIndent = false;
        return;
    }

    void string() {
        while (peek() != '"' && !isAtEnd()) advance();

        if (isAtEnd()) {
            error();
            return;
        }

        advance();
        addToken(STRING, code.substr(start + 1, (current - 1) - (start + 1)));
    }

    void identifier() {
        while (isIdentifierCharacter(peek())) {
            advance();
        }

        std::string text = code.substr(start, current - start);
        if (keywords.find(text) != keywords.end()) {
            addToken(keywords[text]);
        }
        else {
            addToken(IDENTIFIER, text);
        }
    }

    void number() {
        while (isNumeric(peek())) advance();
        addToken(NUMBER, code.substr(start, current - start));
    }

    void addToken(TokenType t) {
        tokens.push_back(Token(t, ""));
    }

    void addToken(TokenType t, std::string v) {
        tokens.push_back(Token(t, v));
    }

    void error() {
        throw std::runtime_error("Error parsing character");
    }
};

