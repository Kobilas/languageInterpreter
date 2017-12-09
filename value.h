#ifndef VALUE_H_
#define VALUE_H_

#include <string>
#include <iostream>
#include "lexer.h"
#include "parser.h"
#include "typefornode.h"

using std::ostream;
class Value {
private:
    TypeForNode type;
    int iValue;
    std::string	sValue;
public:
    Value(TypeForNode t) : type(t), iValue(0), sValue("") {}
    Value(int i) : type(INT_TYPE), iValue(i), sValue("") {}
    Value(std::string s) : type(STRING_TYPE), iValue(0), sValue(s) {}
    Value() : type(ERROR_TYPE), iValue(), sValue() {};
    /*
    std::ostream& operator<<(std::ostream& os, Value& y) {
    if (y.getType() == INT_TYPE) {
    os << y.getNum();
    }
    else if (y.getType() == STRING_TYPE) {
    os << y.getString();
    }
    return os;
    }
    */
    Value operator +(const Value &l) {
        if (this->type == INT_TYPE)
            return Value(this->iValue + l.iValue);
        else if (this->type == STRING_TYPE)
            return Value(this->sValue + l.sValue);
    }
    Value operator -(const Value &l) {
        return Value(this->iValue - l.iValue);
    }
    Value operator *(const Value &l) {
        if (this->type == INT_TYPE)
            return Value(this->iValue*l.iValue);
    }
    bool getValue(int& toChange) {
        if (type == INT_TYPE) {
            toChange = iValue;
            return true;
        }
        else {
            return false;
        }
    }
    bool getValue(std::string& toChange)
    {
        if (type == STRING_TYPE)
        {
            toChange = sValue;
            return true;
        }
        else
        {
            return false;
        }
    }
    TypeForNode getType() const
    {
        return type;
    }
    void setType(TypeForNode newType)
    {
        type = newType;
    }
    int getNum() const
    {
        return iValue;
    }
    void setNum(int newIValue)
    {
        iValue = newIValue;
    }
    string getString()
    {
        return sValue;
    }
    void setString(string newSValue)
    {
        sValue = newSValue;
    }
};
#endif
