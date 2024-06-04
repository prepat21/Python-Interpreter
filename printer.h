#pragma once

#include <iostream>
#include <vector>
#include "visitor.h"
#include "statement.h"
#include "token.h"


class Printer : Visitor<void> {
public:
    Printer() {}

    void print(std::vector<Statement*> stmts) {
        for (size_t i = 0; i < stmts.size(); i++) {
            if (i != 0) {
                std::cout << "\n";
            }
            stmts[i]->accept(this);
        }
    }

    void visitVarStatement(Var* stmt) override {
        std::cout << "(Var, " << stmt->name.value << ", ";
        stmt->initial->accept(this);
        std::cout << ")";
    };

    void visitBlockStmt(Block* stmt) override {
        std::cout << "(Block, ";
        for (size_t i = 0; i < stmt->statements.size(); i++) {
            if (i != 0) {
                std::cout << ", ";
            }
            stmt->statements[i]->accept(this);
        }
        std::cout << ")";
    };

    void visitExpressionStmt(Expression* stmt) override {
        std::cout << "(Expression, ";
        stmt->expr->accept(this);
        std::cout << ")";
    };

    void visitFunctionStmt(Function* stmt) override {
        std::cout << "(Function " << stmt->name.value << ", ";
        std::cout << "(";
        for (auto p : stmt->params) {
            std::cout << p.value << ", ";
        }
        std::cout << "), ";
        std::cout << "\n(\n";
        print(stmt->body);
        std::cout << "\n))";
    };

    void visitIfStmt(If* stmt) override {
        std::cout << "(If, ";
        stmt->condition->accept(this);
        std::cout << ", ";
        stmt->thenBranch->accept(this);
        std::cout << ", ";
        stmt->elseBranch->accept(this);
        std::cout << ")";
    };
    void visitPrintStatement(Print* stmt) override {
        std::cout << "(Print, ";
        for (size_t i = 0; i < stmt->exprs.size(); i++) {
            if (i != 0) {
                std::cout << ", ";
            }
            stmt->exprs[i]->accept(this);
        }
        std::cout << ")";
    };
    void visitReturnStmt(Return* stmt) override {
        std::cout << "(Return, ";
        stmt->value->accept(this);
        std::cout << ")";
    };

    void visitAssignExpr(Assign* expr) override {};
    void visitBinaryExpr(Binary* expr) override {
        std::cout << "(" << tokenNames[expr->op.type] << ", ";
        expr->left->accept(this);
        std::cout << ", ";
        expr->right->accept(this);
        std::cout << ")";
    };
    void visitCallExpr(Call* expr) override {
        std::cout << "(";
        std::cout << expr->callee.value << ", ";
        std::cout << "(";

        bool comma = false;
        for (Expr* e : expr->args) {
            if (comma) {
                std::cout << ", ";
            }
            e->accept(this);
            comma = true;
        }
        std::cout << ")";
        std::cout << ")";
    };
    void visitGroupingExpr(Grouping* expr) override {
        std::cout << "(";
        expr->expression->accept(this);
        std::cout << ")";
    };
    void visitLiteralExpr(Literal* expr) override {
        std::cout << "(" << tokenNames[expr->token.type] << ", " << expr->value << ")";
    };
    void visitLogicalExpr(Logical* expr) override {
        std::cout << "(" << tokenNames[expr->op.type] << ", ";
        expr->left->accept(this);
        std::cout << ", ";
        expr->right->accept(this);
        std::cout << ")";
    };
    void visitUnaryExpr(Unary* expr) override {
        std::cout << "(" << tokenNames[expr->op.type] << ", ";
        expr->right->accept(this);
        std::cout << ")";
    };
    void visitVariableExpr(Variable* expr) override {
    };


};
