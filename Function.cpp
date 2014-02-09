#include "stdafx.h"
#include "Function.h"
#include <sstream>
namespace lang
{

Function::Function(std::string* name,std::vector<std::string>* argList,lang::scope* scope,int index)
{
    this->type = new Type(PreType::_Function);
    this->name = name;
    this->argList = argList;
    this->scope = scope;//std::make_shared<lang::scope>(*scope);
    this->index = index;
}
Function::Function(Function* f,lang::scope* this_scope)
{
    this->type = f->type;
    this->name = f->name;
    this->argList = f->argList;
    this->scope = f->scope;
    this->index = f->index;
    this->thisscope = this_scope;
    this->ptr = nullptr;
}
Function::~Function(void)
{
    this->scope->refdec();
    this->scope = nullptr;
    this->ptr = new int(1);
    std::cout<<"‚ª‚×‚±‚ê’†..."<< *name <<std::endl;
}
std::string Function::toString(void)
{
    return "function:" + *name;
}
langObject Function::call(std::vector<langObject>* argList)
{
    auto sc =new lang::scope(this->scope->parsers,this->scope/*.get()*/);
    sc->type = en::scopeType::_function;
    sc->startIndex = this->index;
    if(this->argList->size()!=argList->size()) throw "ˆø”‚Ì”‚ªˆá‚¤";
    for(int i=0;i<this->argList->size();i++)
    {
        sc->variable.add((*this->argList)[i],(*argList)[i]);
    }
    auto buf = sc->run();
    return std::shared_ptr<Object>( buf);
}
langFunction scope::anonymousFunction(int& index)
{
    std::stringstream str;
    str<<"anonymousFunction"<<index;
    //str=;
    std::string* name = new std::string(str.str());
    int funcRead=2;
    std::vector<std::string>* argList = new std::vector<std::string>();
    
    langFunction func = std::make_shared<Function>(name,nullptr,this,index+1);
    for(int i=index+1;i<this->parsers.size();i++)
    {
        auto token = this->parsers[i];
        if(!funcRead)break;
        switch (funcRead)
        {
        case 2://(
            if(token->pEnum == parserEnum::leftparent)funcRead++;else funcRead = 0;
            if(token->pEnum == parserEnum::rightparent)funcRead = 6;
            break;
        case 3://type 
            if(token->pEnum == parserEnum::identifier)funcRead++;else funcRead = 0;
            if(token->pEnum == parserEnum::rightparent)funcRead = 6;
            break;
        case 4://name
            if(token->pEnum == parserEnum::identifier)
            {
                argList->push_back(*token->name);
                funcRead++;
            }
            else funcRead = 0;
            break;
        case 5://, or )
            if(token->pEnum == parserEnum::comma)funcRead-=2;
            else if(token->pEnum == parserEnum::rightparent)funcRead++; 
            else funcRead = 0;
            break;
        case 6://{
            funcRead = 0;
            func->argList = argList;
            func->index = i;
                //func = new Function(name, argList, this,i);
            //this->runner->variable.add(*funcName,std::make_shared<langFunction>(*funcName,argList,this->runner));
            break;
        }
    }
    if(func->argList == nullptr)throw "syntax error";
    this->refinc();
    index = this->blockSkip(func->index);
    return std::shared_ptr<Function>(func);
}
}
