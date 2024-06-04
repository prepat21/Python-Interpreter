#pragma once

#include <algorithm>
#include <string>
#include <stack>
#include <vector>
#include <iostream>
#include "environment.h"
#include "visitor.h"
#include "statement.h"
#include "object.h"
#include "return.h"

class Interpreter: public Visitor<Object*> {
public:
    Interpreter() {
        global_env = new Environment();
    }

    void run(std::vector<Statement*> stmts) {
        for (Statement* s : stmts) {
            s->accept(this);
        }
    };

    Object* evaluate(Statement* stmt) {
        return stmt->accept(this);
    }

    Object* evaluate(Expr* expr) {
        return expr->accept(this);
    }

    Object* visitVarStatement(Var* stmt) {
        if (stackframe_env.size() != 0) {
            stackframe_env.top()->set(stmt->name.value, stmt->initial->accept(this));
        }
        else {
            global_env->set(stmt->name.value, stmt->initial->accept(this));
        }
        return nullptr;
    };
    
    Object* visitBlockStmt(Block* stmt) {
        for (auto s : stmt->statements) {
            s->accept(this);
        }
        return nullptr;
    };
    
    Object* visitExpressionStmt(Expression* stmt) {
        stmt->accept(this);
        return nullptr;
    };
    
    Object* visitFunctionStmt(Function* stmt) {
        global_env->set(stmt->name.value, stmt);
        return nullptr;
    };

    Object* visitIfStmt(If* stmt) {
        Object* conditional_obj = evaluate(stmt->condition);
        Boolean* conditional = dynamic_cast<Boolean*>(conditional_obj);
        if (conditional == nullptr) {
            error();
        }

        if (conditional != nullptr && conditional->value) {
            evaluate(stmt->thenBranch);
        }
        else if (stmt->elseBranch != nullptr) {
            evaluate(stmt->elseBranch);
        }

        return nullptr;
    };
    Object* visitPrintStatement(Print* stmt) {
        for (size_t i = 0; i < stmt->exprs.size(); i++) {
            if (i != 0) {
                std::cout << " ";
            }
            Object* v = evaluate(stmt->exprs[i]);
            std::cout << v->toString();
        }
        std::cout << "\n";
        return nullptr;
    };
    Object* visitReturnStmt(Return* stmt) {
        Object* return_obj = evaluate(stmt->value);
        throw ReturnException(return_obj);
    };
    Object* visitAssignExpr(Assign* expr) {
        return nullptr;
    };
    Object* visitBinaryExpr(Binary* expr) {
        Object* rhs_obj = evaluate(expr->right);
        Object* lhs_obj = evaluate(expr->left);
        Integer* lhs_int = dynamic_cast<Integer*>(lhs_obj);
        Integer* rhs_int = dynamic_cast<Integer*>(rhs_obj);
        Boolean* lhs_bool = dynamic_cast<Boolean*>(lhs_obj);
        Boolean* rhs_bool = dynamic_cast<Boolean*>(rhs_obj);

        Boolean* res = nullptr;;
        switch (expr->op.type) {
        case EQUAL_TO:
            if (lhs_int != nullptr && rhs_int != nullptr) {
                return new Boolean(lhs_int->value == rhs_int->value);
            }
            else if (lhs_bool != nullptr && rhs_bool != nullptr) {
                return new Boolean(lhs_bool->value == rhs_bool->value);
            }
        case NOT_EQUAL_TO:
            if (lhs_int != nullptr && rhs_int != nullptr) {
                return new Boolean(lhs_int->value != rhs_int->value);
            }
            else if (lhs_bool != nullptr && rhs_bool != nullptr) {
                return new Boolean(lhs_bool->value != rhs_bool->value);
            }
        case GREATER_THAN:
            if (lhs_int != nullptr && rhs_int != nullptr) {
                return new Boolean(lhs_int->value > rhs_int->value);
            }
        case LESS_THAN:
            if (lhs_int != nullptr && rhs_int != nullptr) {
                return new Boolean(lhs_int->value < rhs_int->value);
            }
        case GREATER_THAN_EQUAL_TO:
            if (lhs_int != nullptr && rhs_int != nullptr) {
                return new Boolean(lhs_int->value >= rhs_int->value);
            }
        case LESS_THAN_EQUAL_TO:
            if (lhs_int != nullptr && rhs_int != nullptr) {
                return new Boolean(lhs_int->value <= rhs_int->value);
            }
        case MINUS:
            if (lhs_int != nullptr && rhs_int != nullptr) {
                return new Integer(lhs_int->value - rhs_int->value);
            }
        case PLUS:
            if (lhs_int != nullptr && rhs_int != nullptr) {
                return new Integer(lhs_int->value + rhs_int->value);
            }
        case DIVIDE:
            if (lhs_int != nullptr && rhs_int != nullptr) {
                return new Integer((int)(lhs_int->value / rhs_int->value));
            }
        case MULTIPLY:
            if (lhs_int != nullptr && rhs_int != nullptr) {
                return new Integer((int)(lhs_int->value * rhs_int->value));
            }
        }
        error();
        return nullptr;
    };
    Object* visitCallExpr(Call* expr) {
        std::string name = expr->callee.value;

        Function* func = global_env->get_function(name);

        std::vector<Object*> args;
        for (auto a : expr->args) {
            args.push_back(evaluate(a));
        }

        return run_function(func, args);
    };
    Object* visitGroupingExpr(Grouping* expr) {
        return evaluate(expr->expression);
    };
    Object* visitLiteralExpr(Literal* expr) {
        switch (expr->token.type) {
        case TRUE:
            return new Boolean(true);
        case FALSE:
            return new Boolean(false);
        case NONE:
            return new None();
        case IDENTIFIER:
            if (stackframe_env.size() != 0 && stackframe_env.top()->exists(expr->token.value)) {
                return stackframe_env.top()->get(expr->token.value);
            }
            return global_env->get(expr->token.value);
        case NUMBER:
            return new Integer(std::stoi(expr->token.value));
        case STRING:
            return new String(expr->token.value);
        }
        error();
        return nullptr;
    };
    Object* visitLogicalExpr(Logical* expr) {
        Object* rhs_obj = evaluate(expr->right);
        Object* lhs_obj = evaluate(expr->left);
        Boolean* lhs_bool = dynamic_cast<Boolean*>(lhs_obj);
        Boolean* rhs_bool = dynamic_cast<Boolean*>(rhs_obj);

        if (lhs_bool != nullptr && rhs_bool != nullptr) {
            switch (expr->op.type) {
            case AND:
                return new Boolean(lhs_bool->value && rhs_bool->value);
            case OR:
                return new Boolean(lhs_bool->value || rhs_bool->value);
            }
        }
        error();
        return nullptr;
    };
    Object* visitUnaryExpr(Unary* expr) {
        Object* rhs_obj = evaluate(expr->right);
        Integer* rhs_int = dynamic_cast<Integer*>(rhs_obj);
        Boolean* rhs_bool = dynamic_cast<Boolean*>(rhs_obj);

        switch (expr->op.type) {
        case MINUS:
            if (rhs_int == nullptr) { error(); return nullptr; }
            else {
                return new Integer(-rhs_int->value);
            }
        case NOT:
            if (rhs_bool == nullptr) { error(); return nullptr; }
            else if (rhs_bool != nullptr && rhs_bool->value) { return new Boolean(false); }
            else {
                return new Boolean(true);
            }
        }

        error();
        return nullptr;
    };
    Object* visitVariableExpr(Variable* expr) {
        return nullptr;
    };

    void create_stackframe(std::vector<Token> params, std::vector<Object*> args) {
        stackframe_env.push(new Environment());

        size_t i = 0;
        while (i < params.size()) {
            stackframe_env.top()->set(params.at(i).value, args.at(i));
            i++;
        }
    }

    void pop_stackframe() {
        stackframe_env.pop();
    }

private:
    Environment* global_env;
    std::stack<Environment*> stackframe_env;
    std::stack<int> tempInteger;

    Object* run_function(Function* f, std::vector<Object*> args) {
        if (args.size() != f->params.size()) {
            throw std::runtime_error("wrong sized arguments");
        }
        try {
            this->create_stackframe(f->params, args);
            this->run(f->body);
        }
        catch (ReturnException r) {
            this->pop_stackframe();
            return r.return_object;
        }

        this->pop_stackframe();
        return new None();
    }


    void error() {
        throw std::runtime_error("Error interpreter");
    }
};

