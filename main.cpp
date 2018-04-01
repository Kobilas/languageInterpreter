#include <iostream>
#include <fstream>
#include <string>
#include <map>
using namespace std;

#include "parser.h"
#include "lexer.h"

int lineNumber;
string fileMessage;
int errorcount = 0;
bool hasError = false;

void messageStart(int linenum) {
    cout << fileMessage << linenum + 1 << ":";
}

void error(int linenum, const string& message) {
    hasError = true;
    errorcount++;
    messageStart(linenum);
    cout << message << endl;
}

map<string, int> intSymbols;
map<string, string> strSymbols;
map<int, string> errors;

void postOrder(ParseTree * t) {
    if (t == 0)
        return;
    if (t->getLeft() != 0) {
        postOrder(t->getLeft());
    }
    if (t->getRight() != 0) {
        postOrder(t->getRight());
    }
    t->eval();
}

int main(int argc, char *argv[])
{
    ifstream file;
    istream *in = &cin;

    for (int i = 1; i < argc; i++) {
        string arg(argv[i]);
        if (in != &cin) {
            cout << "TOO MANY FILES" << endl;
            return 0;
        }
        fileMessage = arg + ":";
        file.open(arg);
        if (file.is_open() == false) {
            cout << arg << " FILE NOT FOUND" << endl;
            return 0;
        }

        in = &file;
    }

    ParseTree *tree = Prog(in);

    if (tree == 0 || errorcount > 0) {
        return 1;
    }
    postOrder(tree);
    map<int, string>::iterator it;
    for(it = errors.begin(); it != errors.end(); it++)
    {
        error(it->first, it->second);
    }
    return 0;
}


