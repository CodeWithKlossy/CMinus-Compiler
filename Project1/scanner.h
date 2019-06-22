//´Ê·¨·ÖÎö
#ifndef SCANNER_H
#define SCANNER_H

#include <fstream>
#include <string>
#include "compiler.h"

using std::fstream;
using std::string;

typedef enum {
    START,
    INSLASH,
    INASTERISK1,
    INASTERISK2,
    INID,
    INNUM,
    INSMAL,
    INBIG,
    INEQ,
    INEXCLAM,
    DONE
} STATE;

struct Token {
    string str;
    TokenType type;
};

static Token reservedWords[]={{"else", ELSE},{"if", IF},{"int", INT},{"return", RETURN},{"void", VOID},{"while", WHILE}};

class Scanner {
   public:
    Scanner(fstream &_source) : source(_source) { curt = line.begin(); }
	/*Scanner(Scanner &)=delete;*/
    Token getToken();

   private:
    fstream &source;
    string line;
    Token token;
    string::iterator curt;

    char getChar();
    void ungetChar();
    TokenType lookupReserved(const Token &t);
};
#endif