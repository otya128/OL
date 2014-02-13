#pragma once
#include <vector>
#include "parseObj.h"
#include <map>
#include <memory>
#include "variable.h"
//#include "GC.h"
//#include "Function.h"
//#include "Object.h"
//#define std::shared_ptr<Function> Function
namespace lang
{
    class Function;
    ///typedef std::shared_ptr<Function> langFunction;
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
#define isClass() _this
    //std::map<std::string,langObject>
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
        scope(std::vector<parseObj*>& v);
        scope(std::vector<parseObj*>& v,scope* parent,langClassObject _this);
        langObject run(void);
        ~scope(void);
        int parentSkip(int index);
        int blockSkip(int index,int j=0);
        langObject eval(langObject object,int& index,int opera = 17,bool isbinaryoperation=false);
        langFunction anonymousFunction(int& index);
        langClassObject _this;
    };
    langObject BuidInFunction(std::string name,std::vector<langObject> arg);
}
