#ifndef PARSER_H_
#define PARSER_H_

#include <iostream>
using std::istream;

#include <map>
#include <string>
using std::string;
using std::stoi;

#include "lexer.h"

extern void error(int linenum, const string& message);

enum TypeForNode { INT_TYPE, STRING_TYPE, ERROR_TYPE };

extern std::map<string, int> intSymbols;
extern std::map<string, string> strSymbols;
extern bool hasError;

class ParseTree {
    int linenumber;
    ParseTree *left;
    ParseTree *right;
public:
    ParseTree(int n, ParseTree *lineNum = 0, ParseTree *r = 0) : linenumber(n), left(lineNum), right(r) { }
    virtual ~ParseTree() { }
    ParseTree* getLeft() const { return left; }
    ParseTree* getRight() const { return right; }
    int getLineNumber() const { return linenumber; }
    virtual TypeForNode GetType() const { return ERROR_TYPE; }
    virtual int GetIntValue() const { throw "no integer value"; }
    virtual string GetStringValue() const { throw "no string value"; }
    virtual void eval() { }
};
class StatementList : public ParseTree {
public:
    StatementList(ParseTree *first, ParseTree *rest) : ParseTree(0, first, rest) { }
};
class Addition : public ParseTree {
public:
    int val1 = 0, val2 = 0;
    int value = 0;
    int lineNum = 0;
    string str1, str2, str;
    TypeForNode typeN;
    ParseTree* tree;
    ParseTree* tree2;
    Addition(int line, ParseTree *para1, ParseTree *para2) : ParseTree(line, para1, para2) {
        lineNum = para1->getLineNumber();
        typeN = para1->GetType();
        tree = para1;
        tree2 = para2;
        if (!(tree->GetType() == tree2->GetType() && (tree->GetType() == INT_TYPE || tree->GetType() == STRING_TYPE))) {
            typeN = ERROR_TYPE;
        }
    }
    void eval() {
        if (typeN == ERROR_TYPE) {
            error(lineNum, "type error");
        }
        if (tree->GetType() == tree2->GetType()) {
            typeN = tree->GetType();
            if (tree->GetType() == INT_TYPE) {
                val1 = tree->GetIntValue();
                val2 = tree2->GetIntValue();
                value = val1 + val2;
            }
            else if (tree->GetType() == STRING_TYPE) {
                str1 = tree->GetStringValue();
                str2 = tree2->GetStringValue();
                str = str1 + str2;
            }
        }
    }
    TypeForNode GetType() const { return typeN; }
    int GetIntValue() const { return value; }
    string GetStringValue() const { return str; }
};
class Subtraction : public ParseTree {
public:
    int val1 = 0, val2 = 0, lineNum = 0;
    int value = 0;
    ParseTree* tree;
    ParseTree* tree2;
    TypeForNode typeN;
    Subtraction(int line, ParseTree *para1, ParseTree *para2) : ParseTree(line, para1, para2) {
        lineNum = para1->getLineNumber();
        tree = para1;
        tree2 = para2;
        typeN = para1->GetType();
        if (!(tree->GetType() == INT_TYPE && tree2->GetType() == INT_TYPE)) {
            error(lineNum, "type error");
            typeN = ERROR_TYPE;
        }
        else {
        }
    }
    void eval() {
        if (tree->GetType() == INT_TYPE && tree2->GetType() == INT_TYPE) {
            val1 = tree->GetIntValue();
            val2 = tree2->GetIntValue();
            value = val1 - val2;
        }
    }
    TypeForNode GetType() const { return typeN; }
    int GetIntValue() const { return value; }
};
class Multiplication : public ParseTree {
public:
    int val1 = 0, val2 = 0, lineNum = 0;
    int value = 0;
    string str1, str2;
    string str = "";
    TypeForNode typeN;
    ParseTree* tree;
    ParseTree* tree2;
    Multiplication(int line, ParseTree *para1, ParseTree *para2) : ParseTree(line, para1, para2) {
        lineNum = para1->getLineNumber();
        typeN = para1->GetType();
        tree = para1;
        tree2 = para2;
        if (!((tree->GetType() == INT_TYPE && INT_TYPE == tree2->GetType()) || (tree->GetType() == STRING_TYPE && tree2->GetType() == INT_TYPE)
            || (tree->GetType() == INT_TYPE && tree2->GetType() == STRING_TYPE)))
        {
            error(lineNum, "type error");
            typeN = ERROR_TYPE;
        }
    }
    void eval() {
        if (typeN != ERROR_TYPE) {
            if (tree->GetType() == INT_TYPE && INT_TYPE == tree2->GetType()) {
                val1 = tree->GetIntValue();
                val2 = tree2->GetIntValue();
                value = val1 * val2;
            }
            else {
                if (tree->GetType() == STRING_TYPE && tree2->GetType() == INT_TYPE) {
                    str1 = tree->GetStringValue();
                    val2 = tree2->GetIntValue();
                }
                else if (tree->GetType() == INT_TYPE && tree2->GetType() == STRING_TYPE) {
                    str1 = tree2->GetStringValue();
                    val2 = tree->GetIntValue();
                }
                typeN = STRING_TYPE;
                for (int i = 0; i < val2; i++) {
                    str += str1;
                }
            }
        }
    }

    TypeForNode GetType() const { return typeN; }
    int GetIntValue() const { return value; }
    string GetStringValue() const { return str; }

};
class Division : public ParseTree {
public:
    int val1 = 0, val2 = 0, lineNum = 0;
    int value = 0;
    string str1, str2;
    string str = "";
    ParseTree* tree;
    ParseTree* tree2;
    TypeForNode typeN;
    Division(int line, ParseTree *para1, ParseTree *para2) : ParseTree(line, para1, para2) {
        lineNum = para1->getLineNumber();
        tree = para1;
        tree2 = para2;
        typeN = para1->GetType();
        if (!((tree->GetType() == INT_TYPE && INT_TYPE == tree2->GetType()) || (tree->GetType() == STRING_TYPE && tree2->GetType() == STRING_TYPE))) {
            error(lineNum, "type error");
            typeN = ERROR_TYPE;
        }
    }
    void eval() {
        if (tree->GetType() == INT_TYPE && INT_TYPE == tree2->GetType()) {
            val1 = tree->GetIntValue();
            val2 = tree2->GetIntValue();
            value = val1 / val2;
        }
        else if (tree->GetType() == STRING_TYPE && tree2->GetType() == STRING_TYPE) {
            str1 = tree->GetStringValue();
            str2 = tree2->GetStringValue();
            int x = 0;
            string temp = "";
            bool found = false;
            for (unsigned i = 0; i < str1.length(); i++) {
                temp += str1.substr(i, 1);
                if (!(str1.substr(i, 1) == str2.substr(x, 1)) || found) {
                    str += temp;
                    temp = "";
                    x = 0;
                }
                else {
                    if (temp == str2) {
                        found = true;
                        temp = "";
                    }
                    x++;
                }
            }
        }

    }
    TypeForNode GetType() const { return typeN; }
    int GetIntValue() const { return value; }
    string GetStringValue() const { return str; }
};
class IntegerConstant : public ParseTree {
    int value;
public:
    IntegerConstant(const Token& tok) : ParseTree(tok.GetLinenum()) {
        value = stoi(tok.GetLexeme());
    }
    TypeForNode GetType() const { return INT_TYPE; }
    int GetIntValue() const { return value; }
};
class StringConstant : public ParseTree {
    string value;
public:
    StringConstant(const Token& tok) : ParseTree(tok.GetLinenum()) {
        value = tok.GetLexeme().substr(1, tok.GetLexeme().length() - 2);
    }
    TypeForNode GetType() const { return STRING_TYPE; }
    string GetStringValue() const { return value; }
};
class PrintValue : public ParseTree {
public:
    string sval = "";
    int ival = 0;
    bool newLine = false;
    ParseTree* ex;
    PrintValue(ParseTree *expr, Token t) : ParseTree(t.GetLinenum(), expr) {
        ex = expr;
        if (t.GetTokenType() == T_PRINTLN) {
            newLine = true;
        }
    }
    void eval() {
        if (ex->GetType() == ERROR_TYPE || hasError) {
            return;
        }
        if (ex->GetType() == STRING_TYPE) {
            sval = ex->GetStringValue();
        }
        else if (ex->GetType() == INT_TYPE) {
            ival = ex->GetIntValue();
        }
        std::cout << sval;
        if (ex->GetType() == INT_TYPE) {
            std::cout << ival;
        }
        if (newLine) {
            std::cout << std::endl;
        }
    }
};
class SetValue : public ParseTree {
public:
    Token t;
    int ival = 0;
    string sval = "";
    TypeForNode tree;
    int lineNum;
    ParseTree* ex;
    SetValue(Token tok, ParseTree *expr) : ParseTree(tok.GetLinenum(), expr) {
        lineNum = tok.GetLinenum();
        tree = expr->GetType();
        t = tok;
        ex = expr;
    }
    void eval() {
        if (ex->GetType() == INT_TYPE) {
            ival = ex->GetIntValue();
        }
        else {
            sval = ex->GetStringValue();
        }
        if (intSymbols.find(t.GetLexeme()) == intSymbols.end() && strSymbols.find(t.GetLexeme()) == strSymbols.end()) {
            return;
        }
        if (tree == INT_TYPE) {
            if (intSymbols.find(t.GetLexeme()) != intSymbols.end()) {
                intSymbols[t.GetLexeme()] = ival;
            }
            else {
                error(lineNum, "type error");
            }
        }
        else if (tree == STRING_TYPE) {
            if (strSymbols.find(t.GetLexeme()) != strSymbols.end()) {
                strSymbols[t.GetLexeme()] = sval;
            }
            else {
                error(lineNum, "type error");
            }
        }
        else
        {
            error(lineNum, "type error");
        }
    }

};
class Integer : public ParseTree {
    string name;
public:
    Integer(Token t) : ParseTree(t.GetLinenum()) {
        name = t.GetLexeme();
    }

    TypeForNode GetType() const { return INT_TYPE; }
    int GetIntValue() const { return intSymbols[name]; }

};
class String : public ParseTree {
    string name;
public:
    String(Token t) : ParseTree(t.GetLinenum()) {
        name = t.GetLexeme();
    }

    TypeForNode GetType() const { return STRING_TYPE; }
    string GetStringValue() const { return strSymbols[name]; }
};
class Error : public ParseTree {
public:
    Error(Token t) : ParseTree(t.GetLinenum()) {
    }
    TypeForNode GetType() const { return ERROR_TYPE; }
};

extern ParseTree * Prog(istream* in);
extern ParseTree * StmtList(istream* in);
extern ParseTree * Stmt(istream* in);
extern ParseTree * Decl(istream* in);
extern ParseTree * Set(istream* in);
extern ParseTree * Print(istream* in);
extern ParseTree * Expr(istream* in);
extern ParseTree * Term(istream* in);
extern ParseTree * Primary(istream* in);

#endif
