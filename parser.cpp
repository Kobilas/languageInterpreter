
#include <string>
using std::string;

#include "parser.h"

class ParserToken {
private:
    Token	tok;
    bool	pushedBack;

public:
    ParserToken() : pushedBack(false) {}

    Token getToken(istream *in) {
        if (pushedBack) {
            pushedBack = false;
            return tok;
        }

        return ::getToken(in);
    }
    void pushbackToken(const Token& t) {
        if (pushedBack) {
            throw "you cannot push back two tokens!";
        }
        tok = t;
        pushedBack = true;
    }
} ParserToken;

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

ParseTree *
Prog(istream* in)
{
    return StmtList(in);
}

ParseTree *
StmtList(istream* in)
{
    ParseTree *stmt = Stmt(in);
    if (stmt == 0)
        return 0;

    return new StatementList(stmt, StmtList(in));
}

ParseTree *	Stmt(istream* in) {
    Token tok = ParserToken.getToken(in);
    ParserToken.pushbackToken(tok);

    if (tok == T_INT || tok == T_STRING)
        return Decl(in);

    if (tok == T_SET)
        return Set(in);

    if (tok == T_PRINT || tok == T_PRINTLN)
        return Print(in);

    return 0;
}

ParseTree *	Decl(istream* in) {
    Token type = ParserToken.getToken(in);
    Token ident = ParserToken.getToken(in);

    if (ident != T_ID) {
        error(ident.GetLinenum(), "identifier required");
        return 0;
    }

    if (ParserToken.getToken(in) != T_SC) {
        error(ident.GetLinenum(), "semicolon required");
        return 0;
    }

    return new DeclStatement(type.GetLinenum(), type == T_INT ? INT_TYPE : STRING_TYPE, ident.GetLexeme());

    return 0;
}

ParseTree *	Set(istream* in) {
    Token st = ParserToken.getToken(in);
    Token ident = ParserToken.getToken(in);

    if (ident != T_ID) {
        error(ident.GetLinenum(), "identifier required");
        return 0;
    }

    ParseTree *ex = Expr(in);
    if (ex == 0) {
        error(ident.GetLinenum(), "expression required");
        return 0;
    }

    if (ParserToken.getToken(in) != T_SC) {
        error(ident.GetLinenum(), "semicolon required");
        return 0;
    }

    //return new SetStatement(st.GetLinenum(), ident.GetLexeme(), ex);
    return 0;
}

ParseTree *	Print(istream* in) {
    Token st = ParserToken.getToken(in);

    ParseTree *ex = Expr(in);
    if (ex == 0) {
        error(st.GetLinenum(), "expression required");
        return 0;
    }

    if (ParserToken.getToken(in) != T_SC) {
        error(st.GetLinenum(), "semicolon required");
        return 0;
    }

    return new PrintValue(ex, st);
    return 0;
}

ParseTree *Expr(istream* in) {
    ParseTree *t1 = Term(in);
    if (t1 == 0)
        return 0;
    for (;;) {
        Token op = ParserToken.getToken(in);
        if (op != T_PLUS && op != T_MINUS) {
            ParserToken.pushbackToken(op);
            return t1;
        }
        ParseTree *t2 = Expr(in);
        if (t2 == 0) {
            error(op.GetLinenum(), "expression required after + or - operator");
            return 0;
        }
        if (op == T_PLUS)
            t1 = new Addition(op.GetLinenum(), t1, t2);
        else
            t1 = new Subtraction(op.GetLinenum(), t1, t2);
    }
    return 0;
}

ParseTree *	Term(istream* in) {
    ParseTree *t1 = Primary(in);
    if (t1 == 0)
        return 0;
    for (;;) {
        Token op = ParserToken.getToken(in);
        if (op != T_STAR && op != T_SLASH) {
            ParserToken.pushbackToken(op);
            return t1;
        }
        ParseTree *t2 = Term(in);
        if (t2 == 0) {
            error(op.GetLinenum(), "expression required after * or / operator");
            return 0;
        }
        if (op == T_STAR)
            t1 = new Multiplication(op.GetLinenum(), t1, t2);
        else
            t1 = new Division(op.GetLinenum(), t1, t2);
    }
    return 0;
}

ParseTree *	Primary(istream* in) {
    Token tok = ParserToken.getToken(in);

    if (tok == T_ICONST)
        return new IntegerConstant(tok);

    if (tok == T_SCONST)
        return new StringConstant(tok);

    //if( tok == T_ID )
    //return new Identifier(tok);

    if (tok == T_LPAREN) {
        ParseTree *ex = Expr(in);
        if (ex == 0) {
            error(tok.GetLinenum(), "expression expected");
            return 0;
        }

        tok = ParserToken.getToken(in);
        if (tok != T_RPAREN) {
            error(tok.GetLinenum(), "right paren expected");
            return 0;
        }

        return ex;
    }

    error(tok.GetLinenum(), "primary expected");
    return 0;
}

