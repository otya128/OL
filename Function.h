#pragma once
#include "lang.h"
#ifdef CPP11
#include <thread>
#endif
#include "scope.h"
#include "GC.h"
//#include "Object.h"
namespace lang
{
    class Function : public Object
    {
    public:
        //�֐��Ɋ֘A�t����ꂽstd::thread
#ifdef CPP11
        std::thread* thread;
#endif
        bool working;
        //std::string* getString();
        //void setString(std::string* i);
        std::string name;
        std::vector<std::string>* argList;
        int index;
        scope* scope;
        lang::scope* thisscope;
        Function(std::string name,std::vector<std::string>* argList,lang::scope* scope,int index);
        Function(Function* f,lang::scope* this_scope);
        virtual ~Function(void);
        virtual std::string toString();
        virtual langObject call(std::vector<langObject>* argList);
        virtual langObject ctorcall(std::vector<langObject>* argList);
    };
    langObject (*FuncTable[])(langObject, std::vector<langObject>); 
    class SpecialFunction : public Function
    {
    public:
        int id;
        SpecialFunction(int id_) : Function("built-in function",nullptr,nullptr,-1), id(id_)
        {
            this->type->TypeEnum = _BuiltFunc;
            this->type->name = "built-in function";
            //lang::gc->uncontroll(this);
            //delete this->type;
        }/*
         SpecialFunction(langObject _t,SpecialFunction* sp) : id(sp->id),_this(_t) ,Function("built-in function",nullptr,nullptr,-1)
         {
         }*/
        virtual langObject call(langObject _this, std::vector<langObject>* argList)
        {
            return (*FuncTable[this->id])(_this, *argList);
        }
        virtual ~SpecialFunction(void);
    };
    //typedef std::shared_ptr<Function> langFunction;
}
