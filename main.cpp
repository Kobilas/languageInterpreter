#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include "parser.h"
#include "lexer.h"

int lineNumber;
string fileMessage;
int errorcount = 0;

void messageStart(int linenum) {
	cout << fileMessage << linenum+1 << ":";
}

void error(int linenum, const string& message) {
	++errorcount;
	messageStart(linenum);
	cout << "Syntax error " << message << endl;
}

void semanticError(int linenum, const string& message) {
	messageStart(linenum);
	cout << message << endl;
}

//map<string,Variable> SymbolTable;

int
main(int argc, char *argv[])
{
	ifstream file;
	istream *in = &cin;

	for( int i=1 ; i < argc; i++ ) {
		string arg(argv[i]);
		if( in != &cin ) {
			cout << "TOO MANY FILES" << endl;
			return 0;
		}
		fileMessage = arg + ":";
		file.open(arg);
		if( file.is_open() == false ) {
			cout << arg << " FILE NOT FOUND" << endl;
			return 0;
		}

		in = &file;
	}

	ParseTree *tree = Prog( in );

	if( tree == 0 || errorcount > 0 ) {
		// there was some kind of parse error
		return 1;
	}
    postOrder(tree);

	//extern map<string,Variable>	symbolTable;

	return 0;
}


