#pragma once
#include <vector>
#include "parseObj.h"
#include <map>
#include <memory>
#include "variable.h"
namespace lang
{
class RuntimeObject
{
public:
    Object* object;
    int index;
    RuntimeObject(Object* obj,int i)
    {
        this->object = obj;
        this->index = i;
    }
};
#pragma once
namespace en
{
enum scopeStatus
{
    none,iden,var,_for,_if,
};
}

struct comparer
{
    public:
    bool operator()(const std::string x, const std::string y)
    {
         return x.compare(y)>0;
    }
};
        langObject const NULLOBJECT = newObject();//std::make_shared<Object>();
//std::map<std::string,std::shared_ptr<Object>>
class scope
{
private:
    std::vector<parseObj*> parsers;
    int startIndex;
    int index;
public:
    variable variable;
    scope(std::vector<parseObj*> v);
    scope(std::vector<parseObj*> v,scope* parent);
    void run(void);
    ~scope(void);
    int parentSkip(int index);
    int blockSkip(int index);
    std::shared_ptr<Object> eval(std::shared_ptr<Object> object,int& index,int opera = 17,bool isbinaryoperation=false);
};
#pragma once
langObject BuidInFunction(std::string name,std::vector<langObject> arg);
}
