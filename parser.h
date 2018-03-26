
#ifndef PARSER_H_
#define PARSER_H_

#include <iostream>
#include <map>
using std::istream;

#include <string>
using std::string;
using std::stoi;

#include "lexer.h"

extern void error(int linenum, const string& message);

enum TypeForNode {
    INT_TYPE,
    STRING_TYPE,
    ERROR_TYPE
};

class ParseTree {
    int	linenumber;
    ParseTree *left;
    ParseTree *right;

public:
    ParseTree(int n, ParseTree *l = 0, ParseTree *r = 0) : linenumber(n), left(l), right(r) {}
    virtual ~ParseTree() {}

    ParseTree* getLeft() const {
        return left;
    }
    ParseTree* getRight() const {
        return right;
    }
    int getLineNumber() const {
        return linenumber;
    }

    virtual TypeForNode GetType() const {
        return ERROR_TYPE;
    }
    virtual int GetIntValue() const {
        throw "no integer value";
    }
    virtual string GetStringValue() const {
        throw "no string value";
    }
    virtual void eval() {}
};

class StatementList : public ParseTree {
public:
    StatementList(ParseTree *first, ParseTree *rest) : ParseTree(0, first, rest) {}
};

class Addition : public ParseTree {
public:
    int value1, value2;
    int value;
    string str1, str2, str;
    TypeForNode toe;
    Addition(int line, ParseTree *para1, ParseTree *para2) : ParseTree(line, para1, para2) {
        if (para1->GetType() == para2->GetType()) {
            toe = para1->GetType();
            if (toe == INT_TYPE) {
                value1 = para1->GetIntValue();
                value2 = para2->GetIntValue();
                value = value1 + value2;
            }
            else if (toe == STRING_TYPE) {
                str1 = para1->GetStringValue();
                str2 = para2->GetStringValue();
                str = str1 + str2;
            }
        }
    }

    TypeForNode GetType() const {
        return toe;
    }
    int GetIntValue() const {
        return value;
    }
    string GetStringValue() const {
        return str;
    }
};

class Subtraction : public ParseTree {
public:
    int value1, value2;
    int value;
    TypeForNode toe;
    Subtraction(int line, ParseTree *para1, ParseTree *para2) : ParseTree(line, para1, para2) {
        toe = para1->GetType();
        if (toe == INT_TYPE) {
            value1 = para1->GetIntValue();
            value2 = para2->GetIntValue();
            value = value1 - value2;
        }
    }
    TypeForNode GetType() const {
        return toe;
    }
    int GetIntValue() const {
        return value;
    }
};

class Multiplication : public ParseTree {
public:
    int value1, value2;
    int value;
    string str1, str2;
    string str = "";
    TypeForNode toe;
    Multiplication(int line, ParseTree *para1, ParseTree *para2) : ParseTree(line, para1, para2) {
        toe = para1->GetType();
        if (toe == INT_TYPE && para1->GetType() == para2->GetType()) {
            value1 = para1->GetIntValue();
            value2 = para2->GetIntValue();
            value = value1 * value2;
        }
        else if (toe == STRING_TYPE && para2->GetType() == INT_TYPE) {
            str1 = para1->GetStringValue();
            value2 = para2->GetIntValue();
        }
        else if (toe == INT_TYPE && para2->GetType() == STRING_TYPE) {
            str1 = para2->GetStringValue();
            value2 = para1->GetIntValue();
        }
        toe = STRING_TYPE;
        for (int i = 0; i < value2; i++) {
            str += str1;
        }
    }
    TypeForNode GetType() const { return toe; }
    int GetIntValue() const { return value; }
    string GetStringValue() const {
        return str;
    }
};

class Division : public ParseTree {
public:
    int value1, value2;
    int value;
    string str1, str2;
    string str = "";
    TypeForNode toe;
    Division(int line, ParseTree *para1, ParseTree *para2) : ParseTree(line, para1, para2) {
        toe = para1->GetType();
        if (toe == INT_TYPE && para1->GetType() == para2->GetType()) {
            value1 = para1->GetIntValue();
            value2 = para2->GetIntValue();
            value = value1 / value2;
        }
        else if (toe == STRING_TYPE && para2->GetType() == STRING_TYPE) {
            str1 = para1->GetStringValue();
            str2 = para2->GetStringValue();
            int x = 0;
            string temp = "";
            bool found = false;
            for (unsigned i = 0; i < str1.length(); i++) {
                //std::cout << str1.substr(i, 1) << " " << str1  << " " << i << std::endl;
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
    TypeForNode GetType() const {
        return toe;
    }
    int GetIntValue() const { return value; }
    string GetStringValue() const { return str; }

};

class IntegerConstant : public ParseTree {
    int	value;

public:
    IntegerConstant(const Token& tok) : ParseTree(tok.GetLinenum()) {
        value = stoi(tok.GetLexeme());
    }

    TypeForNode GetType() const { return INT_TYPE; }
    int GetIntValue() const { return value; }
};

class StringConstant : public ParseTree {
    string	value;

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
    int ival;
    bool newLine = false;
    TypeForNode toe;

    PrintValue(ParseTree *expr, Token t) : ParseTree(t.GetLinenum(), expr) {
        toe = expr->GetType();
        if (toe == STRING_TYPE) {
            sval = expr->GetStringValue();
        }
        else if (toe == INT_TYPE) {
            ival = expr->GetIntValue();
        }
        if (t.GetTokenType() == T_PRINTLN) {
            newLine = true;
        }
    }

    void eval() {
        std::cout << sval;
        if (toe == INT_TYPE) {
            std::cout << ival;
        }
        if (newLine) {
            std::cout << std::endl;
        }
    }

};

class SetValue : public ParseTree {
public:
    SetValue(int line, ParseTree *expr) : ParseTree(line, expr) {

    }

};

class Integer : public ParseTree {
    string name;
public:
    Integer(Token t) : ParseTree(t.GetLinenum()) {
        name = t.GetLexeme();
    }

};

class String : public ParseTree {
    string name;
public:
    String(Token t) : ParseTree(t.GetLinenum()) {
        name = t.GetLexeme();
    }

};

class Error : public ParseTree {
public:
    Error(Token t) : ParseTree(t.GetLinenum()) {

    }

};

class Id : public ParseTree {
public:
    Id(Token t) : ParseTree(t.GetLinenum()) {
    }
};
    
extern void postOrder(ParseTree *t);
extern ParseTree *	Prog(istream* in);
extern ParseTree *	StmtList(istream* in);
extern ParseTree *	Stmt(istream* in);
extern ParseTree *	Decl(istream* in);
extern ParseTree *	Set(istream* in);
extern ParseTree *	Print(istream* in);
extern ParseTree *	Expr(istream* in);
extern ParseTree *	Term(istream* in);
extern ParseTree *	Primary(istream* in);


#endif /* PARSER_H_ */
