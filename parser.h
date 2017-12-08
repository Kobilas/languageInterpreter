
#ifndef PARSER_H_
#define PARSER_H_

#include <iostream>
#include <map>
using std::istream;

#include <string>
using std::string;
using std::stoi;

#include "lexer.h"
#include "value.h"

extern void error(int linenum, const string& message);
extern std::map<string, Value> SymbolTable;

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
    void eval() {}
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
    TypeForNode typeN;
    Addition(int line, ParseTree *para1, ParseTree *para2) : ParseTree(line, para1, para2) {
        if (para1->GetType() == para2->GetType()) {
            typeN = para1->GetType();
            if (typeN == INT_TYPE) {
                value1 = para1->GetIntValue();
                value2 = para2->GetIntValue();
                value = value1 + value2;
            }
            else if (typeN == STRING_TYPE) {
                str1 = para1->GetStringValue();
                str2 = para2->GetStringValue();
                str = str1 + str2;
            }
        }
    }

    TypeForNode GetType() const {
        return typeN;
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
    TypeForNode typeN;
    Subtraction(int line, ParseTree *para1, ParseTree *para2) : ParseTree(line, para1, para2) {
        typeN = para1->GetType();
        if (typeN == INT_TYPE) {
            value1 = para1->GetIntValue();
            value2 = para2->GetIntValue();
            value = value1 - value2;
        }
    }
    TypeForNode GetType() const {
        return typeN;
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
    TypeForNode typeN;
    Multiplication(int line, ParseTree *para1, ParseTree *para2) : ParseTree(line, para1, para2) {
        typeN = para1->GetType();
        if (typeN == INT_TYPE && para1->GetType() == para2->GetType()) {
            value1 = para1->GetIntValue();
            value2 = para2->GetIntValue();
            value = value1 * value2;
        }
        else if (typeN == STRING_TYPE && para2->GetType() == INT_TYPE) {
            str1 = para1->GetStringValue();
            value2 = para2->GetIntValue();
        }
        else if (typeN == INT_TYPE && para2->GetType() == STRING_TYPE) {
            str1 = para2->GetStringValue();
            value2 = para1->GetIntValue();
        }
        typeN = STRING_TYPE;
        for (int i = 0; i < value2; i++) {
            str += str1;
        }
    }
    TypeForNode GetType() const {
        return typeN;
    }
    int GetIntValue() const {
        return value;
    }
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
    TypeForNode typeN;
    Division(int line, ParseTree *para1, ParseTree *para2) : ParseTree(line, para1, para2) {
        typeN = para1->GetType();
        if (typeN == INT_TYPE && para1->GetType() == para2->GetType()) {
            value1 = para1->GetIntValue();
            value2 = para2->GetIntValue();
            value = value1 / value2;
        }
        else if (typeN == STRING_TYPE && para2->GetType() == STRING_TYPE) {
            str1 = para1->GetStringValue();
            str2 = para2->GetStringValue();
            int x = 0;
            string temp = "";
            bool found = false;
            for (unsigned int i = 0; i < str1.length(); i++) {
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
        return typeN;
    }
    int GetIntValue() const {
        return value;
    }
    string GetStringValue() const {
        return str;
    }

};
class IntegerConstant : public ParseTree {
    int value;
public:
    IntegerConstant(const Token& tok) : ParseTree(tok.GetLinenum()) {
        value = stoi(tok.GetLexeme());
    }
    TypeForNode GetType() const {
        return INT_TYPE;
    }
    int GetIntValue() const {
        return value;
    }
};
class StringConstant : public ParseTree {
    string value;
public:
    StringConstant(const Token& tok) : ParseTree(tok.GetLinenum()) {
        value = tok.GetLexeme().substr(1, tok.GetLexeme().length() - 2);
    }
    TypeForNode GetType() const {
        return STRING_TYPE;
    }
    string GetStringValue() const {
        return value;
    }
};

class PrintValue : public ParseTree {
public:
    string sval = "";
    int ival;
    bool newLine = false;
    TypeForNode typeN;
    PrintValue(ParseTree *expr, Token t) : ParseTree(t.GetLinenum(), expr) {
        typeN = expr->GetType();
        if (typeN == STRING_TYPE) {
            sval = expr->GetStringValue();
        }
        else if (typeN == INT_TYPE) {
            ival = expr->GetIntValue();
        }
        if (t.GetTokenType() == T_PRINTLN) {
            newLine = true;
        }
    }
    void eval() {
        std::cout << sval;
        if (typeN == INT_TYPE) {
            std::cout << ival;
        }
        if (newLine) {
            std::cout << std::endl;
        }
    }

};
class DeclStatement : public ParseTree {
    TypeForNode	type;
    string id;
public:
    DeclStatement(int line, TypeForNode ty, string id) : ParseTree(line), type(ty), id(id) {}
    int findSemanticErrors() const {
        if (SymbolTable.find(id) != SymbolTable.end()) {
            semanticError(this->getLineNumber(), "variable " + id + " was already declared");
            return 1;
        }
        SymbolTable[id] = Variable(type);
        return 0;
    }
    Value eval() {
        return SymbolTable[id];
    }
    TypeForNode getType() {
        SymbolTable[id].getType();
    };

    class SetStatement : public ParseTree {
        string id;

    public:
        SetStatement(int line, string id, ParseTree *ex) : ParseTree(line, ex), id(id) {}
        int countSet() const { return 1; }

        int findSemanticErrors() const {
            if (SymbolTable.find(id) == SymbolTable.end()) {
                semanticError(this->getLineNumber(), "variable " + id + " is used before being declared");
                return 1;
            }
            return 0;
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