#include "stdafx.h"
#include "Class.h"
#include "scope.h"
#include "lang.h"
#include "Function.h"
#include "langException.h"
namespace lang
{
    Class::Class(std::string name,int index,membertype member,lang::scope* scope)
    {
        this->scope = scope;
        this->scope->refinc();
        this->type = new Type(PreType::_Class, (char*)"class");
        this->name = name;
        this->index = index;
        this->member = member;
    }

    std::string Class::toString()
    {
        return "class:" + this->name;
    }
    Class::~Class(void)
    {
        if(this->type->TypeEnum == PreType::_Class)
        {delete this->member;
        this->scope->refdec();}//delete this->name;
#if _DEBUG
        if(lang::gc_view)std::cout<<"remove"<<this<<this->type->name<<std::endl;
#endif
        //delete this->type->name;
    }
    ClassObject::ClassObject(Class* type) : Class(type->name,type->index,type->member,type->scope)
    {
        thisscope = new lang::scope(type->scope->parsers, type->scope,this);
        this->scope = type->scope;
        this->thisscope->refinc();
        this->type->TypeEnum = _ClassObject;
        foreach_(var_ i in_ *this->member)//C# style foreach
        {
            if(i.second->type->TypeEnum == _Function)
            {
                auto buf = new Function((langFunction)i.second,this->thisscope);
                buf->scope = thisscope;
                this->thisscope->variable.add(i.first, buf);
            }
            else
                this->thisscope->variable.add(i.first, i.second);
        }
    }

    std::string ClassObject::toString(void)
    {
        if(this->thisscope->variable["ToString"] is _Function)
        {
            auto arg = new std::vector<langObject>;//—áŠOˆ—‚Å‹t‚Éç’·‚É‚È‚é—á
            try
            {
                auto result = ((langFunction)this->thisscope->variable["ToString"])->call(arg);
                delete arg;
                return result->toString();
            }
            catch(...)
            {
                delete arg;
                throw;
            }
        }
    }
    ClassObject::~ClassObject(void)
    {
        this->thisscope->refdec();
        //this->scope->refdec();//scope‚¶‚á‚È‚­‚Äthisscope‚Å‚Í
    }
}
