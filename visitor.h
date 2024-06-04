#pragma once

class Statement;
class Expr;

// statements
class Block;
class Expression;
class Function;
class If;
class Print;
class Return;
class Var;

// expressions
class Assign;
class Binary;
class Call;
class Get;
class Grouping;
class Literal;
class Logical;
class Set;
class Super;
class This;
class Unary;
class Variable;

template <class T>
class Visitor {
public:
    virtual T visitVarStatement(Var* stmt) = 0;
    virtual T visitBlockStmt(Block* stmt) = 0;
    virtual T visitExpressionStmt(Expression* stmt) = 0;
    virtual T visitFunctionStmt(Function* stmt) = 0;
    virtual T visitIfStmt(If* stmt) = 0;
    virtual T visitPrintStatement(Print* stmt) = 0;
    virtual T visitReturnStmt(Return* stmt) = 0;

    virtual T visitAssignExpr(Assign* expr) = 0;
    virtual T visitBinaryExpr(Binary* expr) = 0;
    virtual T visitCallExpr(Call* expr) = 0;
    virtual T visitGroupingExpr(Grouping* expr) = 0;
    virtual T visitLiteralExpr(Literal* expr) = 0;
    virtual T visitLogicalExpr(Logical* expr) = 0;
    virtual T visitUnaryExpr(Unary* expr) = 0;
    virtual T visitVariableExpr(Variable* expr) = 0;
};

