#pragma once
#include "scope.h"
//#include "Object.h"
namespace lang
{
class Function : public Object
{
public:
	//std::string* getString();
	//void setString(std::string* i);
    //名前
    std::string name;
    //引数
    std::vector<std::string>* argList;
    int index;
    //スコープ
    scope* scope;
    lang::scope* thisscope;
	Function(std::string name,std::vector<std::string>* argList,lang::scope* scope,int index);
	Function(Function* f,lang::scope* this_scope);
	~Function(void);
    virtual std::string toString();
    langObject call(std::vector<langObject>* argList);
};

//typedef std::shared_ptr<Function> langFunction;
}
