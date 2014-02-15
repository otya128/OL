#pragma once
#include <vector>
#include "object.h"
#include "lang.h"
namespace lang
{
    class Array :
        public Object
    {
    public:
        std::vector<langObject> ary;
        Array(int length);
        Array(std::vector<langObject>&a);
        virtual ~Array(void);
        virtual std::string toString(void);
    };
}

