#pragma once
#include <map>
#include <memory>
#include "Object.h"
//#include "scope.h"
namespace lang
{
class variable
{
public:
    std::map<std::string,std::shared_ptr<Object>> _variable;
    
    variable *parentVariable;
    variable(variable* s);
    variable(void);
    ~variable(void);
    std::shared_ptr<Object> search(std::string name)
    {
        if(this->_variable.find(name) != this->_variable.end())
            return this->_variable[name];
        else
        {
                                               //lang::NULLOBJECT
            if(this->parentVariable == nullptr) return nullptr;
            return this->parentVariable->search(name);
        }
    }
    std::shared_ptr<Object> operator[](std::string name)
    {
        if(this->_variable.find(name) != this->_variable.end())
            return this->_variable[name];
        else if(this->parentVariable != nullptr)return this->parentVariable->search(name);
    }
    std::shared_ptr<Object> set(std::string name,std::shared_ptr<Object> object)
    {
        if(this->_variable.find(name) != this->_variable.end()){
            this->_variable[name] = object;return this->_variable[name];}
        else
        {
                                               //lang::NULLOBJECT
            if(this->parentVariable == nullptr) return nullptr;
            return this->parentVariable->set(name,object);
        }
    }
    void add(std::string name,std::shared_ptr<Object> object)
    {
        this->_variable[name] = object;
    }
    /*std::shared_ptr<Object> operator=(std::string name)
    {
        if(this->_variable.find(name) != this->_variable.end())
            return this->_variable[name];
        else if(this->parentVariable != nullptr)return this->parentVariable->setsearch(name);
    }*/
};
}
