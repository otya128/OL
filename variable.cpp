#pragma once
#include "stdafx.h"
#include "variable.h"
//#include "Object.h"
namespace lang{
    variable::variable(variable* s)
    {
        this->_variable = std::map<std::string,langObject>();
        this->childVariable = nullptr;
        this->parentVariable = s;
        s->childVariable = this;
    }


    variable::variable(void)
    {
        //this->_variable = std::map<std::string,langObject>();
        this->childVariable = nullptr;
        this->parentVariable = nullptr;
    }
    variable::~variable(void)
    {
        if(this->childVariable != nullptr)
            this->childVariable->parentVariable = nullptr;
    }

    langObject variable::search(std::string name)
    {
        if(this->_variable.find(name) != this->_variable.end())
            return this->_variable[name];
        else
        {
            //lang::NULLOBJECT
            if(this->parentVariable == nullptr) return lang::NULLOBJECT;
            return this->parentVariable->search(name);
        }
    }
    bool variable::definedVar(std::string name)
    {
        if(this->_variable.find(name) != this->_variable.end())
            return true;
        else
        {
            //lang::NULLOBJECT
            if(this->parentVariable == nullptr) return false;
            return this->parentVariable->definedVar(name);
        }
    }
    langObject variable::operator[](std::string name)
    {
        if(this->_variable.find(name) != this->_variable.end())
            return this->_variable[name];
        else if(this->parentVariable != nullptr)return this->parentVariable->search(name);
        return lang::NULLOBJECT;
    }
    langObject variable::set(std::string name,langObject object)
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
    void variable::add(std::string name,langObject object)
    {
        this->_variable[name] = object;
    }
}
