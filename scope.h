#pragma once
#include <vector>
#include "parseObj.h"
#include <map>
#include <memory>
#include "variable.h"
//#include "Function.h"
//#include "Object.h"
namespace lang
{
class Function;
typedef std::shared_ptr<Function> langFunction;
/*class RuntimeObject
{
public:
    Object* object;
    int index;
    RuntimeObject(Object* obj,int i)
    {
        this->object = obj;
        this->index = i;
    }
};*/
namespace en
{
enum scopeStatus
{
    none,iden,var,_for,_if,
};
enum returnStatus
{
    none_,_return,_break
};
enum scopeType
{
    _none_,_function,for_,
};
}

//std::map<std::string,std::shared_ptr<Object>>
class scope
{
private:
    int refcount;
public:
    void refinc();
    void refdec();
    void del();
    std::vector<parseObj*> parsers;
    int startIndex;
    int index;
    en::scopeType type;
    en::returnStatus status;
    variable variable;
    scope(std::vector<parseObj*> v);
    scope(std::vector<parseObj*> v,scope* parent);
    langObject run(void);
    ~scope(void);
    int parentSkip(int index);
    int blockSkip(int index,int j=0);
    std::shared_ptr<Object> eval(std::shared_ptr<Object> object,int& index,int opera = 17,bool isbinaryoperation=false);
    langFunction anonymousFunction(int& index);
};
langObject BuidInFunction(std::string name,std::vector<langObject> arg);
}
