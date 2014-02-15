#include "stdafx.h"
#include "Array.h"


namespace lang
{
    Array::Array(int length)
    {
        this->type = new Type(_Array);
        this->ary = std::vector<langObject>(length);
    }

    Array::Array(std::vector<langObject>&a)
    {
        this->type = new Type(_Array);
        this->ary = a;
    }

    std::string Array::toString(void)
    {
        std::string result;
        result += "{";
        foreach_(var_ i in_ this->ary)
        {
            if(result.size() != 1)result += ",";
            if(i == nullptr) result += "nullptr";
            else result += i->toString();
        }
        result += "}";
        return result;
    }
    Array::~Array(void)
    {
    }
}
