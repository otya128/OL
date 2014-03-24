#ifndef __LANG_VARIABLE_H__
#define __LANG_VARIABLE_H__//#pragma once
#include <map>
#include <memory>
#include "Object.h"
//#include "scope.h"
namespace lang
{
    class variable
    {
    public:
        std::map<std::string,langObject> _variable;

        variable *parentVariable;
        variable *childVariable;
        variable(variable* s);
        variable(void);
        ~variable(void);
        bool definedVar(std::string name);
        langObject search(std::string name);
        langObject operator[](std::string name);
        langObject set(std::string name,langObject object);
        void add(std::string name,langObject object);
        /*langObject operator=(std::string name)
        {
        if(this->_variable.find(name) != this->_variable.end())
        return this->_variable[name];
        else if(this->parentVariable != nullptr)return this->parentVariable->setsearch(name);
        }*/
    };
}
#endif
