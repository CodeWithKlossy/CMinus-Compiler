#ifndef _SYNANALYSIS_H_
#define _SYNANALYSIS_H_

#include <fstream>
#include <iostream>
#include <string>
#include "compiler.h"
#include "scanner.h"

using std::fstream;
using std::string;

class Parser {
public:
	shared_ptr<Node> root;

	Parser(fstream& _source) : scanner(_source) {};
	//Parser(Parser&) = delete;

	void parse();//语法分析
	void print(shared_ptr<Node> root, int indent = 0);//打印语法树

private:
	Scanner scanner;
	Token curtToken;
	bool hasError;

	/**辅助函数**/
	//错误提示
	void syntaxError(string msg) {
		std::cout << msg << std::endl;
		hasError = true;
	}
	//检查当前token
	void checkAndGet() { curtToken = scanner.getToken(); }
	void checkAndGet(TokenType type) {
		if (curtToken.type != type)
			syntaxError("syntax error");
		else
			curtToken = scanner.getToken();
	}

	Node* declaration_list();
	Node* declaration();
	Node* params();
	Node* param_list(Node* node);
	Node* param(Node* node);
	Node* compound_stmt();
	Node* local_declaration();

	Node* statement_list();
	Node* statement();
	Node* expression_stmt();
	Node* selection_stmt();
	Node* iteration_stmt();
	Node* return_stmt();

	Node* expression();
	Node* simple_expression(Node* node);
	Node* additive_expression(Node* node);
	Node* term(Node* node);
	Node* factor(Node* node);
	Node* var();
	Node* call(Node* node);
	Node* args();
	Node* arg_list();
};

#endif