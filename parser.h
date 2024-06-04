#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "token.h"
#include "statement.h"
#include "expression.h"

typedef std::vector<TokenType> TokenTypes;

class Parser {

public:
	Parser(std::vector<Token> tokens) {
		this->tokens = tokens;
	}

	std::vector<Statement*> parse() {
		std::vector<Statement*> statements;
		while (!isAtEnd()) {
			while (match(NEWLINE)) { ; }
			if (check(END)) {
				break;
			}
			statements.push_back(declaration());
		}
		return statements;
	}

private:
	std::vector<Token> tokens;
	int current = 0;

	Statement* declaration() {
		if (match(DEF)) {
			return functionDeclaration();
		}
		if (peek().type == IDENTIFIER && peekNext().type == EQUAL) {
			return varDeclaration();
		}
		return statement();
	}

	Statement* functionDeclaration() {
		Token name = consume(IDENTIFIER);
		std::vector<Token> params;
		consume(LPARAN);

		if (!match(RPARAN)) {
			params.push_back(advance());
			while (match(COMMA)) {
				params.push_back(advance());
			}
			consume(RPARAN);
		}

		consume(COLON);
		consume(NEWLINE);
		consume(INDENT);
		std::vector<Statement*> body;
		body.push_back(declaration());
		while (!check(DEDENT)) {
			while (match(NEWLINE)) { ; }
			if (check(END) || match(DEDENT)) {
				break;
			}
			body.push_back(declaration());
		}
		return new Function(name, params, body);
	}

	Statement* varDeclaration() {
		Token name = consume(IDENTIFIER);

		Expr* initializer = nullptr;
		if (match(EQUAL)) {
			initializer = expression();
		}

		consume(NEWLINE);
		return new Var(name, initializer);
	}

	Statement* statement() {
		if (match(IF)) {
			return ifStatement();
		}
		if (match(PRINT)) {
			return printStatement();
		}
		if (match(RETURN)) {
			return returnStatement();
		}
		return expressionStatement();
	}

	Statement* ifStatement() {
		Expr* conditional = expression();
		consume(COLON);
		consume(NEWLINE);
		consume(INDENT);
		
		Statement* thenBranch = blockStatement();
		Statement* elseBranch = nullptr;
		if (match(ELSE)) {
			consume(COLON);
			consume(NEWLINE);
			consume(INDENT);
			elseBranch = blockStatement();
		}

		return new If(conditional, thenBranch, elseBranch);
	}

	Statement* printStatement() {
		std::vector<Expr*> args = arguments();
		return new Print(args);
	}


	Statement* returnStatement() {
		Token ret = previous();
		Expr* value = nullptr;
		if (match(NEWLINE)) {
			return new Return(ret, value);
		}
		value = expression();
		return new Return(ret, value);
	}

	Statement* blockStatement() {
		std::vector<Statement*> lines;
		while (!match(DEDENT)) {
			while (match(NEWLINE)) { ; }
			lines.push_back(declaration());
			while (match(NEWLINE)) { ; }
		}
		return new Block(lines);
	}

	Statement* expressionStatement() {
		Expr* expr = expression();
		return new Expression(expr);
	}

	Expr* expression() {
		return or_();
	}

	Expr* or_() {
		Expr* expr = and_();
		while (match(OR)) {
			Token op = previous();
			Expr* rhs = and_();
			expr = new Logical(expr, op, rhs);
		}
		return expr;
	}

	Expr* and_() {
		Expr* expr = comparison();
		while (match(AND)) {
			Token op = previous();
			Expr* rhs = comparison();
			expr = new Logical(expr, op, rhs);
		}
		return expr;
	}

	Expr* comparison() {
		Expr* expr = term();
		while (match(TokenTypes{NOT_EQUAL_TO, EQUAL_TO, GREATER_THAN_EQUAL_TO, GREATER_THAN, LESS_THAN, LESS_THAN_EQUAL_TO})) {
			Token op = previous();
			Expr* rhs = term();
			expr = new Binary(expr, op, rhs);
		}
		return expr;
	}

	Expr* term() {
		Expr* expr = factor();
		while (match(TokenTypes{ MINUS, PLUS })) {
			Token op = previous();
			Expr* rhs = factor();
			expr = new Binary(expr, op, rhs);
		}
		return expr;
	}

	Expr* factor() {
		Expr* expr = unary();
		while (match(TokenTypes{ MULTIPLY, DIVIDE })) {
			Token op = previous();
			Expr* rhs = unary();
			expr = new Binary(expr, op, rhs);
		}
		return expr;
	}

	Expr* unary() {
		if (match({ MINUS, NOT })) {
			Token op = previous();
			Expr* rhs = unary();
			return new Unary(op, rhs);
		}
		return call();
	}

	Expr* call() {
		Expr* expr;
		if (check(IDENTIFIER) && peekNext().type == LPARAN) {
			Token name = advance();
			std::vector<Expr*> args = arguments();
			Token paren = previous();
			return new Call(name, paren, args);
		}
		else {
			expr = primary();
		}
		return expr;
	}

	Expr* primary() {
		if (match(TokenTypes{ TRUE, FALSE, NONE })) {
			return new Literal(previous(), "");
		}
		if (match(LPARAN)) {
			Expr* expr = expression();
			consume(RPARAN);
			return new Grouping(expr);
		}
		if (match(TokenTypes{ IDENTIFIER, NUMBER, STRING })) {
			return new Literal(previous(), previous().value);
		}
		error();
		return nullptr;
	}

	std::vector<Expr*> arguments() {
		std::vector<Expr*> args;
		consume(LPARAN);
		while (!match(RPARAN)) {
			if (args.size() > 0) {
				consume(COMMA);
			}
			args.push_back(expression());
		}

		return args;
	}

	void error() {
		std::cout << "\n" << tokenNames[previous().type] << " " << previous().value;
		std::cout << "\n" << tokenNames[peek().type] << " " << peek().value;
		std::cout << "\n" << tokenNames[peekNext().type] << " " << peekNext().value;
		throw std::runtime_error("Error parsing Token: " + tokenNames[peek().type]);
	}

	bool match(std::vector<TokenType> types) {
		for (TokenType t : types) {
			if (check(t)) {
				advance();
				return true;
			}
		}
		return false;
	}

	Token consume(TokenType type) {
		if (check(type)) return advance();

		error();
		return peek();
	}

	bool match(TokenType type) {
		if (check(type)) {
			advance();
			return true;
		}
		return false;
	}
	bool check(TokenType t) {
		if (isAtEnd()) return false;
		return peek().type == t;
	}
	Token advance() {
		if (!isAtEnd()) current++;
		return previous();
	}
	Token peek() {
		return tokens.at(current);
	}
	Token peekNext() {
		if (current + 1 >= tokens.size()) return tokens.at(current);
		return tokens.at(current + 1);
	}
	Token previous() {
		return tokens.at(current - 1);
	}
	bool isAtEnd() {
		return (current >= tokens.size());
	}

};