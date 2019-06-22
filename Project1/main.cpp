#include <fstream>
#include <iostream>
#include <string>
#include "compiler.h"
#include "scanner.h"
#include "SynAnalysis.h"

using std::cout;
using std::endl;
using std::fstream;

int main(int argc, char** argv) {
	fstream source;
	source.open("gcd.c-", std::ios::in);

	Parser parser(source);
	parser.parse();
	cout << "Syntax Tree:" << endl;
	parser.print(parser.root);
	cout << "OVER" << endl;
	system("PAUSE");

	return 0;
}