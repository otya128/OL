#pragma once
#include "lang.h"
#include "Object.h"
#include "variable.h"
namespace lang
{
class scope;
typedef std::vector<std::pair<std::string*,langObject>> membertype_;
typedef membertype_* membertype;
class Class : public Object
{
public:
    membertype member;
    Class(std::string* name,int index,membertype member,scope* scope);
    ~Class(void);
    //名前
    std::string* name;
    lang::scope* scope;
    int index;
    virtual std::string toString();
};
class ClassObject : public Class
{
public:
    //スコープ
    lang::scope* thisscope;
    ClassObject(Class* type);
    ~ClassObject(void);
};
}
