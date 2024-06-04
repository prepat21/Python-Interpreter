#pragma once

#include <vector>
#include "token.h"
#include "expression.h"
#include "visitor.h"
#include "object.h"

class Statement {
public:
	virtual ~Statement() {}
	virtual void accept(Visitor<void>* v) {};
	virtual Object* accept(Visitor<Object*>* v) { return nullptr;  };
};

class Block : public Statement {
public:
	std::vector<Statement*> statements;

	Block(std::vector<Statement*> stmts) {
		statements = stmts;
	}

	void accept(Visitor<void>* v) override {
		v->visitBlockStmt(this);
	}

	Object* accept(Visitor<Object*>* v) override {
		return v->visitBlockStmt(this);
	}
};

class Expression : public Statement {
public:
	Expr* expr;

	Expression(Expr* expr) {
		this->expr = expr;
	}

	void accept(Visitor<void>* v) override {
		v->visitExpressionStmt(this);
	}

	Object* accept(Visitor<Object*>* v) override {
		return v->visitExpressionStmt(this);
	}
};

class Function : public Statement {
public:
	Token name;
	std::vector<Token> params;
	std::vector<Statement*> body;

	Function(Token name, std::vector<Token> params, std::vector<Statement*> body) {
		this->name = name;
		this->params = params;
		this->body = body;
	}

	void accept(Visitor<void>* v) override {
		v->visitFunctionStmt(this);
	}

	Object* accept(Visitor<Object*>* v) override {
		return v->visitFunctionStmt(this);
	}
};

class If : public Statement {
public:
	Expr* condition;
	Statement* thenBranch;
	Statement* elseBranch;

	If(Expr* condition, Statement* thenBranch, Statement* elseBranch) {
		this->condition = condition;
		this->thenBranch = thenBranch;
		this->elseBranch = elseBranch;
	}

	void accept(Visitor<void>* v) override {
		v->visitIfStmt(this);
	}

	Object* accept(Visitor<Object*>* v) override {
		return v->visitIfStmt(this);
	}
};


class Print : public Statement {
public:
	std::vector<Expr*> exprs;

	Print(std::vector<Expr*> exprs) {
		this->exprs = exprs;
	}

	void accept(Visitor<void>* v) override {
		v->visitPrintStatement(this);
	}

	Object* accept(Visitor<Object*>* v) override {
		return v->visitPrintStatement(this);
	}
};

class Return : public Statement {
public:
	Token keyword;
	Expr* value;

	Return(Token keyword, Expr* value) {
		this->keyword = keyword;
		this->value = value;
	}

	void accept(Visitor<void>* v) override {
		v->visitReturnStmt(this);
	}

	Object* accept(Visitor<Object*>* v) override {
		return v->visitReturnStmt(this);
	}
};

class Var : public Statement {
public:
	Token name;
	Expr* initial;

	Var(Token name, Expr* initial) {
		this->name = name;
		this->initial = initial;
	}

	void accept(Visitor<void>* v) override {
		v->visitVarStatement(this);
	}

	Object* accept(Visitor<Object*>* v) override {
		return v->visitVarStatement(this);
	}
};
