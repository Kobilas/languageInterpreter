#include <string>
using std::string;

#include <map>

#include "parser.h"
class ParserToken
{
private:
    Token tok;
    bool pushedBack;
public:
    ParserToken() : pushedBack(false) { }
    Token getToken(istream *in)
    {
        if (pushedBack)
        {
            pushedBack = false;
            return tok;
        }
        return ::getToken(in);
    }
    void pushbackToken(const Token& t)
    {
        if (pushedBack)
        {
            //error
        }
        tok = t;
        pushedBack = true;
    }
} ParserToken;
extern bool hasError;
extern std::map<int, string> errors;
extern std::map<string, int> intSymbols;
extern std::map<string, string> strSymbols;
ParseTree * Prog(istream* in)
{
    return StmtList(in);
}
ParseTree * StmtList(istream* in)
{
    ParseTree *stmt = Stmt(in);
    if (stmt == 0)
    {
        return 0;
    }
    return new StatementList(stmt, StmtList(in));
}
ParseTree * Stmt(istream* in)
{
    Token para;
    ParseTree *p;
    p = Print(in);
    if (p == 0)
    {
        p = Decl(in);
    }
    if (p == 0)
    {
        p = Set(in);
    }
    if (p != 0)
    {
        if ((para = ParserToken.getToken(in)) == T_SC || (para = ParserToken.getToken(in)) == T_DONE)
        {
            return p;
        }
        else
        {
            error(para.GetLinenum(), "Syntax error missing semicolon");
            return 0;
        }
    }
    return 0;
}
ParseTree * Decl(istream* in)
{
    Token para = ParserToken.getToken(in);
    if (para.GetTokenType() == T_INT || para.GetTokenType() == T_STRING)
    {
        bool isInt = false;
        if (para.GetTokenType() == T_INT)
        {
            isInt = true;
        }
        if ((para = ParserToken.getToken(in)).GetTokenType() == T_ID)
        {
            if ((intSymbols.find(para.GetLexeme()) == intSymbols.end()) && (strSymbols.find(para.GetLexeme()) == strSymbols.end()))
            {
                if (isInt)
                {
                    intSymbols[para.GetLexeme()] = 0;
                    return new Integer(para);
                }
                strSymbols[para.GetLexeme()] = "";
                return new String(para);
            }
            errors[para.GetLinenum()] = "variable " + para.GetLexeme() + " was already declared";
            return new Error(para);
        }
        error(para.GetLinenum(), "Syntax error ID required");
        return 0;
    }
    else {
        ParserToken.pushbackToken(para);
        return 0;
    }
    ParserToken.pushbackToken(para);
    return 0;
    para = ParserToken.getToken(in);
    if (para.GetTokenType() == T_ID) {
        return 0;
    }
    return 0;
}
ParseTree * Set(istream* in) {
    Token para = ParserToken.getToken(in);
    if (para.GetTokenType() != T_SET) {
        ParserToken.pushbackToken(para);
        return 0;
    }
    para = ParserToken.getToken(in);
    if (para.GetTokenType() != T_ID) {
        ParserToken.pushbackToken(para);
        error(para.GetLinenum(), "Syntax error ID required");
        return 0;
    }
    if ((intSymbols.find(para.GetLexeme()) == intSymbols.end()) && (strSymbols.find(para.GetLexeme()) == strSymbols.end())) {
        errors[para.GetLinenum()] = "variable " + para.GetLexeme() + " is used before being declared";
    }
    ParseTree *e1 = Expr(in);
    if (e1 != 0) {
        return new SetValue(para, e1);
    }
    error(para.GetLinenum(), "Syntax error expression required");
    return 0;
}
ParseTree * Print(istream* in) {
    Token para = ParserToken.getToken(in);
    if (para.GetTokenType() == T_PRINT || para.GetTokenType() == T_PRINTLN) {
        ParseTree *e1 = Expr(in);
        if (e1 != 0) {
            return new PrintValue(e1, para);
        }
    }
    ParserToken.pushbackToken(para);
    return 0;
}
ParseTree *Expr(istream* in) {
    ParseTree *t1 = Term(in);
    if (t1 == 0)
        return 0;
    for (;;) {
        Token para = ParserToken.getToken(in);
        if (para != T_PLUS && para != T_MINUS) {
            ParserToken.pushbackToken(para);
            return t1;
        }
        ParseTree *t2 = Term(in);
        if (t2 == 0) {
            error(para.GetLinenum(), "expression required after + or - operator");
            return 0;
        }
        if (para == T_PLUS) {
            t1 = new Addition(para.GetLinenum(), t1, t2);
        }
        else
            t1 = new Subtraction(para.GetLinenum(), t1, t2);
    }
    return 0;
}
ParseTree * Term(istream* in) {
    ParseTree *t1 = Primary(in);
    if (t1 == 0)
        return 0;
    for (;;) {
        Token para = ParserToken.getToken(in);
        if (para != T_STAR && para != T_SLASH) {
            ParserToken.pushbackToken(para);
            return t1;
        }
        ParseTree *t2 = Primary(in);
        if (t2 == 0) {
            error(para.GetLinenum(), "term required after * or / operator");
            return 0;
        }
        if (para == T_STAR) {
            t1 = new Multiplication(para.GetLinenum(), t1, t2);
        }
        else
            t1 = new Division(para.GetLinenum(), t1, t2);
    }
    return 0;
}
ParseTree * Primary(istream* in) {
    Token para = ParserToken.getToken(in);
    if (para.GetTokenType() == T_SCONST) {
        return new StringConstant(para);
    }
    if (para.GetTokenType() == T_ICONST) {
        return new IntegerConstant(para);
    }
    if (para.GetTokenType() == T_ID) {
        if (intSymbols.find(para.GetLexeme()) != intSymbols.end()) {
            return new Integer(para);
        }
        else if (strSymbols.find(para.GetLexeme()) != strSymbols.end()) {
            return new String(para);
        }
            errors[para.GetLinenum()] = "variable " + para.GetLexeme() + " is used before being declared";
            return new Error(para);
    }
    if (para.GetTokenType() == T_LPAREN) {
        ParseTree *t1 = Expr(in);
        para = ParserToken.getToken(in);
        if (t1 == 0 || para.GetTokenType() != T_RPAREN) {
            error(para.GetLinenum(), "Syntax error right paren expected");
            return 0;
        }
        else {
            return t1;
        }
    }
    error(para.GetLinenum(), "Syntax error primary expected");
    return 0;
}
