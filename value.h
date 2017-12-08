#ifndef VALUE_H_
#define VALUE_H_

#include <string>
#include "lexer.h"
#include "parser.h"

enum Type { INTEGER, STRING, NONE, ERR };

class Value {
protected:
    Type type;
    int iValue;
    std::string	sValue;
public:
    Value(int i) : type(INTEGER), iValue(i) {}
    Value(std::string s) : type(STRING), iValue(0), sValue(s) {}
    Value() : type(NONE), iValue(), sValue() {};
    friend std::ostream& operator<<(std::ostream&os, const Value&y);
    Value operator +(const Value &l) {
        if (this->type == INTEGER)
            return Value(this->iValue + l.iValue);
        else if (this->type == STRING)
            return Value(this->sValue + l.sValue);
    }
    Value operator -(const Value &l) {
        return Value(this->iValue - l.iValue);
    }
    Value operator *(const Value &l) {
        if (this->type == INTEGER)
            return Value(this->iValue*l.iValue);
    }
    bool getValue(int& toChange) {
        if (type == INTEGER) {
            toChange = iValue;
            return true;
        }
        else {
            return false;
        }
    }
    bool getValue(std::string& toChange) {
        if (type == STRING) {
            toChange = sValue;
            return true;
        }
        else {
            return false;
        }
    }
    Type getType() const {
        return type;
    }
    int getNum()const {
        return iValue;
    }
    string getString() {
        return sValue;
    }
};
inline std::ostream& operator<<(std::ostream& os, const Value& x) {
    if (x.getType() == INTEGER) {
        os << x.iValue;
    }
    else if (x.getType() == STRING) {
        os << x.sValue;
    }
    return os;
}
#endif
