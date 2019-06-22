#ifndef COMPILER_H
#define COMPILER_H

#include <list>
#include <memory>
#include <string>

using std::list;
using std::shared_ptr;
using std::string;

typedef enum {
	//¹Ø¼ü×Ö
    IF,
	ELSE,
    INT,
    RETURN,
    VOID,
    WHILE,
    ID,
    NUM,
    PLUS,//+
    MINUS,//-
    TIMES,//*
    OVER,///
    SEMI,//;
    COMMA,//,
    SMAL,//<
    SMALEQ,//<=
    BIG,//>
    BIGEQ,//>=
    EQ,//==
    NOTEQ,//!=
    ASSIGN,//=
    LPAREN,//(
    RPAREN,//)
    LBRACKET,//[
    RBRACKET,//]
    LBRACE,//{
    RBRACE,//}
    COMMENT,///**/
    ERROR,
    ENDFILE
} TokenType;

typedef enum { Decl, Def, Exp } NodeType;

typedef enum { Var, Arry, Fun, Param, Void, Int, Unkown } DeclType;

typedef enum { Call, Args, Comp, Slec, Iter, Retn } DefType;

typedef enum { Assign, Op, Const, Id, Arry_Elem } ExpType;

union StatementType {
    DeclType decl;
    DefType def;
    ExpType exp;
};

struct Node {
    list<shared_ptr<struct Node>> child;
    shared_ptr<struct Node> sibling;
    int line;

    NodeType nodetype;
    StatementType type;

    //×´Ì¬ÊôÐÔ
    TokenType op;
    int val;
    string name;

    Node(NodeType _nodetype, DeclType _type) : nodetype(_nodetype) {
        type.decl = _type;
    }
    Node(NodeType _nodetype, DefType _type) : nodetype(_nodetype) {
        type.def = _type;
    }
    Node(NodeType _nodetype, ExpType _type) : nodetype(_nodetype) {
        type.exp = _type;
    }
};

#endif  // !COMPILER_H