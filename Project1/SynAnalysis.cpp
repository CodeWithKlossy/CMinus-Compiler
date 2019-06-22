#include "SynAnalysis.h"
#include <string>

using std::cout;
using std::endl;

void Parser::parse() {
	checkAndGet();
	root = shared_ptr<Node>(declaration_list());//存树
}
//打印
void Parser::print(shared_ptr<Node> root, int indent) {
	while (root != nullptr) {
		for (int i = 0; i < indent; ++i)
			cout << "   ";
			

		switch (root->nodetype) {
		case NodeType::Decl:
			switch (root->type.decl) {
			case DeclType::Arry://数组声明
				cout << "Array declaration: " << endl;
				break;
			case DeclType::Fun://函数声明
				cout << "Function declaration: " << endl;
				break;
			case DeclType::Int://int声明
				cout << "Int declaration: " << endl;
				break;
			case DeclType::Param://形参声明
				cout << "Formal parameter declaration: " << endl;
				break;
			case DeclType::Unkown://未知声明
				cout << "Unknown declaration: " << endl;
				break;
			case DeclType::Var://变量声明
				cout << "Variable  declaration: " << endl;
				break;
			case DeclType::Void://void声明
				cout << "Void  declaration: " << endl;
				break;
			default:
				cout << "error" << endl;
				break;
			}
			break;
		case NodeType::Def:
			switch (root->type.def) {
			case DefType::Args://实参
				cout << "Actual parameter: " << endl;
				break;
			case DefType::Call://函数调用
				cout << "Function call: " << endl;
				break;
			case DefType::Comp://复合语句
				cout << "compound statement: " << endl;
				break;
			case DefType::Iter:
				cout << "While: " << endl;
				break;
			case DefType::Retn:
				cout << "Return: " << endl;
				break;
			case DefType::Slec:
				cout << "If: " << endl;
				break;
			default:
				cout << "error" << endl;
				break;
			}
			break;
		case NodeType::Exp:
			switch (root->type.exp) {
			case ExpType::Arry_Elem:
				cout << "Array Element: " << endl;
				break;
			case ExpType::Assign:
				cout << "Assign: " << endl;
				break;
			case ExpType::Const:
				cout << "Const : " << root->val << endl;
				break;
			case ExpType::Id:
				cout << "Identifier: " << root->name << endl;
				break;
			case ExpType::Op:
				cout<<"OP: ";
				switch (root->op) {
					
				case TokenType::PLUS:
					cout << '+';
					break;
				case TokenType::MINUS:
					cout << '-';
					break;
				case TokenType::TIMES:
					cout << '*';
					break;
				case TokenType::OVER:
					cout << '/';
					break;
				case TokenType::EQ:
					cout << "==";
					break;
				case TokenType::BIG:
					cout << ">";
					break;
				case TokenType::BIGEQ:
					cout << ">=";
					break;
				case TokenType::SMAL:
					cout << "<";
					break;
				case TokenType::SMALEQ:
					cout << "<=";
					break;
				case TokenType::NOTEQ:
					cout << "!=";
					break;
				}
				cout << endl;
				break;
			default:
				cout << "error" << endl;
				break;
			}
			break;
		}
		for (auto t : root->child)
			print(t, indent + 1);

		root = root->sibling;
	}
}

/**辅助函数**/

Node* Parser::declaration_list() {
	Node* res = declaration();
	Node* tem = res;

	while ((curtToken.type != TokenType::ENDFILE) &&
		(curtToken.type != TokenType::INT) &&
		(curtToken.type != TokenType::VOID) && !hasError) {
		syntaxError("declaration error");
		checkAndGet();
	}
	while ((curtToken.type == TokenType::INT ||
		curtToken.type == TokenType::VOID) &&
		!hasError) {
		Node* next = declaration();

		if (next != nullptr)
			if (res == nullptr)
				res = tem = next;
			else {
				tem->sibling = shared_ptr<Node>(next);
				tem = next;
			}
	}
	if (curtToken.type != TokenType::ENDFILE)
		syntaxError("syntax error");

	checkAndGet(TokenType::ENDFILE);
	return res;
}

Node* Parser::declaration() {
	Node* typeNode = nullptr;
	Node* idNode = nullptr;
	Node* declNode = nullptr;

	if (curtToken.type == TokenType::INT)
		typeNode = new Node(NodeType::Decl, DeclType::Int);
	else if (curtToken.type == TokenType::VOID)
		typeNode = new Node(NodeType::Decl, DeclType::Void);
	else
		syntaxError("syntax error");

	checkAndGet();
	if (curtToken.type == TokenType::ID) {
		idNode = new Node(NodeType::Exp, ExpType::Id);
		idNode->name = curtToken.str;

		checkAndGet();
		if (curtToken.type == TokenType::LPAREN) {
			declNode = new Node(NodeType::Decl, DeclType::Fun);

			Node* p = nullptr;
			Node* s = nullptr;

			checkAndGet();
			p = params();
			checkAndGet(TokenType::RPAREN);
			s = compound_stmt();

			declNode->child.push_back(shared_ptr<Node>(typeNode));
			declNode->child.push_back(shared_ptr<Node>(idNode));
			declNode->child.push_back(shared_ptr<Node>(p));
			declNode->child.push_back(shared_ptr<Node>(s));
		}
		else if (curtToken.type == TokenType::LBRACKET) {
			declNode = new Node(NodeType::Decl, DeclType::Var);

			Node* elemNode = new Node(NodeType::Exp, ExpType::Arry_Elem);

			checkAndGet();
			checkAndGet(TokenType::NUM);

			Node* indexNode = new Node(NodeType::Exp, ExpType::Const);
			indexNode->val = std::stoi(curtToken.str);

			elemNode->child.push_back(shared_ptr<Node>(idNode));
			elemNode->child.push_back(shared_ptr<Node>(indexNode));
			declNode->child.push_back(shared_ptr<Node>(typeNode));
			declNode->child.push_back(shared_ptr<Node>(elemNode));

			checkAndGet(TokenType::RBRACKET);
			checkAndGet(TokenType::SEMI);
		}
		else if (curtToken.type == TokenType::SEMI) {
			declNode = new Node(NodeType::Decl, DeclType::Var);
			declNode->child.push_back(shared_ptr<Node>(typeNode));
			declNode->child.push_back(shared_ptr<Node>(idNode));
			checkAndGet();
		}
		else
			syntaxError("syntax error");
	}
	else
		syntaxError("syntax error");

	return declNode;
}

Node* Parser::params() {
	Node* paramNode = new Node(NodeType::Decl, DeclType::Param);

	if (curtToken.type == TokenType::VOID) {
		checkAndGet();

		Node* typeNode = new Node(NodeType::Decl, DeclType::Void);
		if (curtToken.type == TokenType::ID)
			paramNode->child.push_back(shared_ptr<Node>(param_list(typeNode)));
		else
			paramNode->child.push_back(shared_ptr<Node>(typeNode));
	}
	else if (curtToken.type == TokenType::INT)
		paramNode->child.push_back(shared_ptr<Node>(param_list(nullptr)));
	else
		syntaxError("parameters error");

	return paramNode;
}

Node* Parser::param_list(Node* node) {
	Node* res = param(node);
	Node* tem = res;

	node = nullptr;
	while (curtToken.type == TokenType::COMMA) {
		checkAndGet();

		Node* next = param(node);
		if (next != nullptr)
			if (res == nullptr)
				res = tem = next;
			else {
				tem->sibling = shared_ptr<Node>(next);
				tem = next;
			}
	}

	return res;
}

Node* Parser::param(Node* node) {
	Node* paramNode = new Node(NodeType::Decl, DeclType::Param);
	Node* typeNode = nullptr;
	Node* idNode = nullptr;

	if (node == nullptr && curtToken.type == TokenType::INT) {
		typeNode = new Node(NodeType::Decl, DeclType::Int);
		checkAndGet();
	}
	else if (node == nullptr && curtToken.type == TokenType::VOID) {
		typeNode = new Node(NodeType::Decl, DeclType::Void);
		checkAndGet();
	}
	else if (node != nullptr)
		typeNode = node;

	if (typeNode != nullptr) {
		paramNode->child.push_back(shared_ptr<Node>(typeNode));

		if (curtToken.type == TokenType::ID) {
			idNode = new Node(NodeType::Exp, ExpType::Id);
			idNode->name = curtToken.str;
			paramNode->child.push_back(shared_ptr<Node>(idNode));
			checkAndGet();
		}
		else
			syntaxError("parameter error");

		if (curtToken.type == TokenType::LBRACKET &&
			paramNode->child.size() > 1) {
			checkAndGet();
			checkAndGet(TokenType::RBRACKET);
		}
	}
	else
		syntaxError("parameter error");

	return paramNode;
}

Node* Parser::compound_stmt() {
	Node* stmtNode = new Node(NodeType::Def, DefType::Comp);

	checkAndGet(TokenType::LBRACE);

	stmtNode->child.push_back(shared_ptr<Node>(local_declaration()));
	stmtNode->child.push_back(shared_ptr<Node>(statement_list()));

	checkAndGet(TokenType::RBRACE);

	return stmtNode;
}

Node* Parser::local_declaration() {
	Node* res = nullptr;
	Node* tem = nullptr;
	Node* declNode = nullptr;

	while (curtToken.type == TokenType::INT ||
		curtToken.type == TokenType::VOID) {
		tem = new Node(NodeType::Decl, DeclType::Var);

		if (curtToken.type == TokenType::INT) {
			tem->child.push_back(
				shared_ptr<Node>(new Node(NodeType::Decl, DeclType::Int)));
			checkAndGet();
		}
		else if (curtToken.type == TokenType::VOID) {
			tem->child.push_back(
				shared_ptr<Node>(new Node(NodeType::Decl, DeclType::Void)));
			checkAndGet();
		}

		if (tem != nullptr && curtToken.type == TokenType::ID) {
			Node* idNode = new Node(NodeType::Exp, ExpType::Id);
			idNode->name = curtToken.str;

			checkAndGet();
			if (curtToken.type == TokenType::LBRACKET) {
				Node* elemNode = new Node(NodeType::Exp, ExpType::Arry_Elem);

				checkAndGet();
				checkAndGet(TokenType::NUM);

				Node* indexNode = new Node(NodeType::Exp, ExpType::Const);
				indexNode->val = stoi(curtToken.str);

				elemNode->child.push_back(shared_ptr<Node>(idNode));
				elemNode->child.push_back(shared_ptr<Node>(indexNode));
				tem->child.push_back(shared_ptr<Node>(elemNode));

				checkAndGet(TokenType::RBRACKET);
				checkAndGet(TokenType::SEMI);
			}
			else if (curtToken.type == TokenType::SEMI)
				checkAndGet();
			else
				checkAndGet();
		}
		else
			syntaxError("syntax error");

		if (tem != nullptr) {
			if (res == nullptr)
				res = declNode = tem;
			else {
				declNode->sibling = shared_ptr<Node>(tem);
				declNode = tem;
			}
		}
	}

	return res;
}

Node* Parser::statement_list() {
	Node* res = statement();
	Node* tem = res;

	while (curtToken.type == TokenType::IF || curtToken.type == TokenType::ID ||
		curtToken.type == TokenType::LBRACE ||
		curtToken.type == TokenType::WHILE ||
		curtToken.type == TokenType::RETURN ||
		curtToken.type == TokenType::SEMI ||
		curtToken.type == TokenType::LPAREN ||
		curtToken.type == TokenType::NUM) {
		Node* next = statement();

		if (next != nullptr)
			if (res == nullptr)
				res = tem = next;
			else {
				tem->sibling = shared_ptr<Node>(next);
				tem = next;
			}
	}

	return res;
}

Node* Parser::statement() {
	Node* t = nullptr;

	if (curtToken.type == TokenType::IF)
		t = selection_stmt();
	else if (curtToken.type == TokenType::WHILE)
		t = iteration_stmt();
	else if (curtToken.type == TokenType::RETURN)
		t = return_stmt();
	else if (curtToken.type == TokenType::LBRACE)
		t = compound_stmt();
	else if (curtToken.type == TokenType::ID ||
		curtToken.type == TokenType::SEMI ||
		curtToken.type == TokenType::LPAREN ||
		curtToken.type == TokenType::NUM)
		t = expression_stmt();
	else {
		syntaxError("syntax error");
		checkAndGet();
	}
	return t;
}

Node* Parser::expression_stmt() {
	Node* res = nullptr;

	if (curtToken.type != TokenType::SEMI)
		res = expression();

	checkAndGet(TokenType::SEMI);

	return res;
}

Node* Parser::selection_stmt() {
	Node* slecNode = new Node(NodeType::Def, DefType::Slec);

	checkAndGet(TokenType::IF);
	checkAndGet(TokenType::LPAREN);
	slecNode->child.push_back(shared_ptr<Node>(expression()));

	checkAndGet(TokenType::RPAREN);
	slecNode->child.push_back(shared_ptr<Node>(statement()));

	if (curtToken.type == TokenType::ELSE) {
		checkAndGet();
		slecNode->child.push_back(shared_ptr<Node>(statement()));
	}

	return slecNode;
}

Node* Parser::iteration_stmt() {
	Node* iterNode = new Node(NodeType::Def, DefType::Iter);

	checkAndGet(TokenType::WHILE);
	checkAndGet(TokenType::LPAREN);

	if (iterNode != nullptr)
		iterNode->child.push_back(shared_ptr<Node>(expression()));
	checkAndGet(TokenType::RPAREN);

	if (iterNode != nullptr)
		iterNode->child.push_back(shared_ptr<Node>(statement()));

	return iterNode;
}

Node* Parser::return_stmt() {
	Node* retnNode = new Node(NodeType::Def, DefType::Retn);

	checkAndGet(TokenType::RETURN);
	if (curtToken.type == TokenType::SEMI)
		checkAndGet();
	else if (retnNode != nullptr) {
		retnNode->child.push_back(shared_ptr<Node>(expression()));
		checkAndGet(TokenType::SEMI);
	}

	return retnNode;
}

Node* Parser::expression() {
	Node* res = var();

	if (res == nullptr)
		res = simple_expression(nullptr);
	else {
		Node* expNode = nullptr;

		if (curtToken.type == TokenType::ASSIGN) {
			expNode = new Node(NodeType::Exp, ExpType::Assign);
			expNode->name = curtToken.str;

			checkAndGet();
			expNode->child.push_back(shared_ptr<Node>(res));
			expNode->child.push_back(shared_ptr<Node>(expression()));

			res = expNode;
		}
		else
			res = simple_expression(res);
	}

	return res;
}

Node* Parser::var() {
	Node* res = nullptr;

	while (curtToken.type == TokenType::ID) {
		Node* expNode = new Node(NodeType::Exp, ExpType::Id);

		expNode->name = curtToken.str;
		checkAndGet();
		if (curtToken.type == TokenType::LBRACKET) {
			checkAndGet();
			Node* tem = expression();
			checkAndGet(TokenType::RBRACKET);
			res = new Node(NodeType::Exp, ExpType::Arry_Elem);
			res->child.push_back(shared_ptr<Node>(expNode));
			res->child.push_back(shared_ptr<Node>(tem));
		}
		else
			res = expNode;
	}

	return res;
}

Node* Parser::simple_expression(Node* node) {
	Node* res = additive_expression(node);
	node = nullptr;

	switch (curtToken.type) {
	case TokenType::SMAL:
	case TokenType::SMALEQ:
	case TokenType::BIG:
	case TokenType::BIGEQ:
	case TokenType::EQ:
	case TokenType::NOTEQ:
		Node* tem = new Node(NodeType::Exp, ExpType::Op);
		tem->op = curtToken.type;
		tem->child.push_back(shared_ptr<Node>(res));

		checkAndGet();
		tem->child.push_back(shared_ptr<Node>(additive_expression(node)));
		res = tem;
	}

	return res;
}

Node* Parser::additive_expression(Node* node) {
	Node* res = term(node);
	node = nullptr;

	while (curtToken.type == TokenType::PLUS ||
		curtToken.type == TokenType::MINUS) {
		Node* tem = new Node(NodeType::Exp, ExpType::Op);

		if (tem != nullptr) {
			tem->op = curtToken.type;
			tem->child.push_back(shared_ptr<Node>(res));

			checkAndGet();
			tem->child.push_back(shared_ptr<Node>(term(node)));
			res = tem;
		}
	}

	return res;
}

Node* Parser::term(Node* node) {
	Node* res = factor(node);
	node = nullptr;

	while (curtToken.type == TokenType::TIMES ||
		curtToken.type == TokenType::OVER) {
		Node* tem = new Node(NodeType::Exp, ExpType::Op);

		tem->op = curtToken.type;
		tem->child.push_back(shared_ptr<Node>(res));

		checkAndGet();
		tem->child.push_back(shared_ptr<Node>(factor(node)));
		res = tem;
	}

	return res;
}

Node* Parser::factor(Node* node) {
	Node* res = nullptr;

	if (node != nullptr)
		if (curtToken.type == TokenType::LPAREN &&
			node->type.exp != ExpType::Arry_Elem)
			res = call(node);
		else
			res = node;
	else
		switch (curtToken.type) {
		case TokenType::LPAREN:
			checkAndGet();
			res = expression();
			checkAndGet(TokenType::RPAREN);
			break;
		case TokenType::NUM:
			res = new Node(NodeType::Exp, ExpType::Const);
			if (res != nullptr)
				res->val = stoi(curtToken.str);
			checkAndGet();
			break;
		case TokenType::ID:
			node = var();
			if (curtToken.type == TokenType::LPAREN &&
				node->type.exp == ExpType::Arry_Elem)
				res = call(node);
			res = node;
			break;
		default:
			syntaxError("syntax error");
			checkAndGet();
			break;
		}

	return res;
}

Node* Parser::call(Node* node) {
	Node* res = new Node(NodeType::Def, DefType::Call);

	if (node != nullptr)
		res->child.push_back(shared_ptr<Node>(node));

	checkAndGet(TokenType::LPAREN);
	if (curtToken.type == TokenType::RPAREN)
		checkAndGet();
	else if (node != nullptr) {
		res->child.push_back(shared_ptr<Node>(args()));
		checkAndGet(TokenType::RPAREN);
	}

	return res;
}

Node* Parser::args() {
	Node* res = new Node(NodeType::Def, DefType::Args);
	Node* expNode = res;

	if (curtToken.type != TokenType::RPAREN) {
		expNode = expression();
		Node* tem = expNode;

		while (curtToken.type == TokenType::COMMA) {
			checkAndGet();

			Node* next = expression();
			if (next != nullptr)
				if (expNode == nullptr)
					expNode = tem = next;
				else {
					tem->sibling = shared_ptr<Node>(next);
					tem = next;
				}
		}
	}
	if (expNode != nullptr)
		res->child.push_back(shared_ptr<Node>(expNode));

	return res;
}

Node* Parser::arg_list() {
	Node* res = new Node(NodeType::Exp, ExpType::Assign);

	if (curtToken.type == TokenType::ID)
		res->name = curtToken.str;

	checkAndGet(TokenType::ID);
	checkAndGet(TokenType::ASSIGN);

	res->child.push_back(shared_ptr<Node>(expression()));

	return res;
}