#include "stdafx.h"
#include "Class.h"
#include "scope.h"
#include "lang.h"
#include "Function.h"
namespace lang
{
Class::Class(std::string* name,int index,membertype member,lang::scope* scope)
{
    this->scope = scope;
    this->type = new Type(PreType::_Class, (char*)name->c_str());
    this->name = name;
    this->index = index;
    this->member = member;
}

std::string Class::toString()
{
    return "class:" + *this->name;
}
Class::~Class(void)
{
}
ClassObject::ClassObject(Class* type) : Class(type->name,type->index,type->member,type->scope)
{
    thisscope = new lang::scope(type->scope->parsers, type->scope,this);
    this->thisscope->refinc();
    this->type->TypeEnum = _ClassObject;
    foreach_(var_ i in_ *this->member)//C# style foreach
    {
        if(i.second->type->TypeEnum == _Function)
        {
            auto buf = new Function((langFunction)i.second,this->thisscope);
            buf->scope = thisscope;
            this->thisscope->variable.add(*i.first, buf);
            
        }
        else
        this->thisscope->variable.add(*i.first, i.second);
    }
}


ClassObject::~ClassObject(void)
{
    this->scope->refdec();
}
}
