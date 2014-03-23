#ifndef __LANG_ARRAY_H__
#define __LANG_ARRAY_H__
//#pragma once
#include <vector>
#include "Object.h"
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
		virtual langObject getMember(std::string &name);
    };
}
#endif
