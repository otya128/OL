#pragma once
#include "stdafx.h"
#include "Object.h"
//#include "parserEnum.h"
#include <sstream>
#include <memory>
#include "GC.h"
#include "Function.h"
#include "Class.h"
#include "langException.h"
#define newInt(a) new Int(a)
#define newString(a) new String(a)
/*#define langObject langObject
#define langInt std::shared_ptr<Int>
#define langString std::shared_ptr<String>*/
namespace lang
{
    がべこれ* gc;
    Object::Object(void* ptr)
    {
        if(lang::gc != nullptr)
        {
            gc->addObject(this);
        }
        this->type = new Type(PreType::_Object);
        this->ptr=ptr;
    }
    Object::Object(void)
    {
        this->ptr = this;
#if _DEBUG
        if(gc_view) std::cout<<"add"<<this<<std::endl;//<<std::endl;
#endif
        if(lang::gc != nullptr)
        {
            gc->addObject(this);
        }
        this->type = (Type*)ObjectType;//new Type(PreType::_Object);
    }

    std::string _toString(Object* arg)
    {
        // return arg->toString();for(i=0;i<10;i++){print(i);}
        if(arg==nullptr)return "null";
        /*if(arg->type->TypeEnum==PreType::_Int)
        {
        std::stringstream ss;
        ss<<(static_cast<Int*>(arg))->getInt();
        return (ss.str());
        }
        if(arg->type->TypeEnum==PreType::_String)
        {
        return *(static_cast<String*>(arg))->getString();
        }
        if(arg->type->TypeEnum==PreType::_Function)
        {
        return *(static_cast<Function*>(arg))->name;
        }*/
        return "";
    }
#define GABEKORE 1
    Object::~Object(void)
    {
#if GABEKORE
#if _DEBUG
        if(gc_view) std::cout<<"がべこれ中..."<<this<<"が滅亡しました...\t"<<this->type->name<<"\t"<<_toString(this)<<std::endl;//<<std::endl;
#endif
#endif
        if(type != (Type*)ObjectType)delete this->type;
        //delete this->ptr;
    }
    SpecialFunction* object_tostr = new SpecialFunction(0);
    SpecialFunction* string_substr = new SpecialFunction(1);
    langObject Object::getMember(std::string& name)
    {
        return object_tostr;
    }
    void* Object::getPointer(void)
    {
        return this->ptr;
    }
    void Object::setPointer(void* ptr)
    {
        this->ptr = ptr;
        return;
    }
    std::string Object::toString()
    {
        return this->type->name;
    }
    Int::Int(int i)
    {
        this->type = new Type(PreType::_Int);
        this->ptr = (void*)new int(i);
        #if _DEBUG
        debuggg = i;
        #endif
    }
    Int::~Int(void)
    {
        //delete this->type;
        delete (int*)this->ptr;
    }
    int Int::getInt(void)
    {
        return *(int*)(this->ptr);
    }
    void Int::setInt(int i)
    {
        delete this->ptr;
        this->ptr = new int(i);
        #if _DEBUG
        debuggg = i;
        #endif
        return;
    }
    std::string Int::toString()
    {
        std::stringstream ss;
        ss<<this->getInt();
        return (ss.str());
    }

    String::String(std::string* i)
    {
        this->type = new Type(PreType::_String);
        this->ptr = (void*)new std::string(*i);

    }
    String::~String(void)
    {
        //delete this->type;
        delete (std::string*)this->ptr;
    }
    std::string* String::getString(void)
    {
        return (std::string*)(this->ptr);
    }
    void String::setString(std::string* i)
    {
        delete this->ptr;
        this->ptr = (void*)i;
        return;
    }
    std::string String::toString(void)
    {
        return *((std::string*)(this->ptr));
    }
    langObject String::getMember(std::string& name)
    {
        if(name == "ToString")
        return object_tostr;
        else if(name == "Substring")
        return string_substr;
    }

    int Int::toInt(langObject obj)
    {
        if(obj->type->TypeEnum==PreType::_Int)
            return (static_cast<Int*>(obj/*.get()*/))->getInt();
        //if(obj->type->TypeEnum==PreType::_Int)
        //return (static_cast<Int*>(obj))->getInt();
        return 0;//変換不可
    }
    #define OPERA2ARG(name) {auto clas = (langClassObject)obj1;\
            auto func = (langFunction)clas->thisscope->variable[name];\
            if(func != nullptr && func is _Function)\
            {\
                auto vec = new std::vector<langObject>();\
                vec->push_back(obj2);\
                try\
                {\
                    auto ret = func->call(vec);\
                    delete vec;\
                    return ret;\
                }\
                catch(...)\
                {\
                    delete vec;\
                    throw;\
                }\
            }\
            else\
                throw langRuntimeException((std::string("関数")+name+"が定義されていません").c_str());}
                
    #define OPERA2ARG2(name) {auto clas = (langClassObject)obj2;\
            auto func = (langFunction)clas->thisscope->variable[name];\
            if(func != nullptr && func is _Function)\
            {\
                auto vec = new std::vector<langObject>();\
                vec->push_back(obj1);\
                try\
                {\
                    auto ret = func->call(vec);\
                    delete vec;\
                    return ret;\
                }\
                catch(...)\
                {\
                    delete vec;\
                    throw;\
                }\
            }\
            else\
                throw langRuntimeException((std::string("関数")+name+"が定義されていません").c_str());}
                /*
                {auto clas = (langClassObject)obj1;
            auto func = (langFunction)clas->thisscope->variable[name];
            if(func != nullptr && func is _Function)
            {
                auto vec = new std::vector<langObject>();
                vec->push_back(obj2);
                try
                {
                    auto ret = func->call(vec);
                    delete vec;
                    return ret;
                }
                catch(...)
                {
                    delete vec;
                    throw;
                }
            }
            else
                throw langRuntimeException((std::string("関数")+name+"が定義されていません").c_str());}
                */
    langObject Object::plus(langObject obj1,langObject obj2)
    {
        switch (obj1->type->TypeEnum)
        {
        case PreType::_Int:
            return newInt(Int::toInt(obj1) + Int::toInt(obj2));
        case _String:
            return newString(&(obj1->toString() + obj2->toString()));
        case PreType::_ClassObject:
            {auto clas = (langClassObject)obj1;
            auto func = (langFunction)clas->thisscope->variable["plus"];
            if(func != nullptr && func is _Function)
            {
                auto vec = new std::vector<langObject>();
                vec->push_back(obj2);
                try
                {
                    auto ret = func->call(vec);
                    delete vec;
                    return ret;
                }
                catch(...)
                {
                    delete vec;
                    throw;
                }
            }
            else
                throw langRuntimeException((std::string("関数plus")+"が定義されていません").c_str());}//OPERA2ARG("plus")
            //break;
        }
        throw langRuntimeException((std::string(obj1->type->name) + "+" + obj2->type->name + "出来ない").c_str());
        //変換不可
    }
    langObject Object::multiply(langObject obj1,langObject obj2)
    {
        switch (obj1->type->TypeEnum)
        {
        case PreType::_Int:
            return newInt(Int::toInt(obj1) * Int::toInt(obj2));
        case PreType::_ClassObject:
            OPERA2ARG("multiply")
        }
        throw langRuntimeException((std::string(obj1->type->name) + "*" + obj2->type->name + "出来ない").c_str());
        //変換不可
    }
    langObject Object::greater(langObject obj1,langObject obj2)
    {
        switch (obj1->type->TypeEnum)
        {
        case PreType::_Int:
            return newInt(Int::toInt(obj1) > Int::toInt(obj2));
        case PreType::_ClassObject:
            OPERA2ARG("greater")
        }
        throw langRuntimeException((std::string(obj1->type->name) + ">" + obj2->type->name + "出来ない").c_str());
        //変換不可
    }
    langObject Object::less(langObject obj1,langObject obj2)
    {
        switch (obj1->type->TypeEnum)
        {
        case PreType::_Int:
            return newInt(Int::toInt(obj1) < Int::toInt(obj2));
        case PreType::_ClassObject:
            OPERA2ARG("less")
        }
        throw langRuntimeException((std::string(obj1->type->name) + "<" + obj2->type->name + "出来ない").c_str());
        //変換不可
    }
    langObject Object::greaterEqual(langObject obj1,langObject obj2)
    {
        switch (obj1->type->TypeEnum)
        {
        case PreType::_Int:
            return newInt(Int::toInt(obj1) >= Int::toInt(obj2));
        case PreType::_ClassObject:
            OPERA2ARG("greaterEqual")
        }
        throw langRuntimeException((std::string(obj1->type->name) + ">=" + obj2->type->name + "出来ない").c_str());
    }
    langObject Object::lessEqual(langObject obj1,langObject obj2)
    {
        switch (obj1->type->TypeEnum)
        {
        case PreType::_Int:
            return newInt(Int::toInt(obj1) <= Int::toInt(obj2));
        case PreType::_ClassObject:
            OPERA2ARG("lessEqual")
        }
        throw langRuntimeException((std::string(obj1->type->name) + "[" + obj1->toString() + "]<=[" + obj2->toString() + "]" + obj2->type->name + "出来ない").c_str());
    }
    langObject Object::equal(langObject obj1,langObject obj2)
    {
        switch (obj1->type->TypeEnum)
        {
        case PreType::_Int:
            return newInt(Int::toInt(obj1) == Int::toInt(obj2));
        case PreType::_ClassObject:
        {
            auto clas = (langClassObject)obj1;
            auto func = (langFunction)clas->thisscope->variable["equal"];
            if(func != nullptr && func is _Function)
            {
                auto vec = new std::vector<langObject>();
                vec->push_back(obj2);
                try
                {
                    auto ret = func->call(vec);
                    delete vec;
                    return ret;
                }
                catch(...)
                {
                    delete vec;
                    throw;
                }
            }
            else
                return newInt(obj1 == obj2);
        }
        default:
            return newInt(obj1 == obj2);
        break;
        }
    }
    langObject Object::modulo(langObject obj1,langObject obj2)
    {
        switch (obj1->type->TypeEnum)
        {
        case PreType::_Int:
            return newInt(Int::toInt(obj1) % Int::toInt(obj2));
        case PreType::_ClassObject:
            OPERA2ARG("modulo")
        }
        throw langRuntimeException((std::string(obj1->type->name) + "%" + obj2->type->name + "出来ない").c_str());
    }
    langObject Object::leftShift(langObject obj1,langObject obj2)
    {
        switch (obj1->type->TypeEnum)
        {
        case PreType::_Int:
            if(obj2 is _ClassObject)
            {
                auto clas = (langClassObject)obj2;
                auto func = (langFunction)clas->thisscope->variable["leftShift"];
                if(func != nullptr && func is _Function)
                {
                    auto vec = new std::vector<langObject>();
                    vec->push_back(obj1);
                    try
                    {
                        auto ret = func->call(vec);
                        delete vec;
                        return ret;
                    }
                    catch(...)
                    {
                        delete vec;
                        throw;
                    }
                }
                else
                    throw langRuntimeException((std::string("関数leftShift")+"が定義されていません").c_str());
            }
            return newInt(Int::toInt(obj1) << Int::toInt(obj2));
        case PreType::_ClassObject:
            OPERA2ARG("leftShift")
        default:

            if(obj2 is _ClassObject)
            {
                auto clas = (langClassObject)obj2;
                auto func = (langFunction)clas->thisscope->variable["leftShift"];
                if(func != nullptr && func is _Function)
                {
                    auto vec = new std::vector<langObject>();
                    vec->push_back(obj1);
                    try
                    {
                        auto ret = func->call(vec);
                        delete vec;
                        return ret;
                    }
                    catch(...)
                    {
                        delete vec;
                        throw;
                    }
                }
                else
                    throw langRuntimeException((std::string("関数leftShift")+"が定義されていません").c_str());
            }
        }
        throw langRuntimeException((std::string(obj1->type->name) + "[" + obj1->toString() + "]<<[" + obj2->toString() + "]" + obj2->type->name + "出来ない").c_str());
    }
    langObject Object::rightShift(langObject obj1,langObject obj2)
    {
        switch (obj1->type->TypeEnum)
        {
        case PreType::_Int:
            return newInt(Int::toInt(obj1) >> Int::toInt(obj2));
        case PreType::_ClassObject:
            OPERA2ARG("rightShift")
        }
        throw langRuntimeException((std::string(obj1->type->name) + ">>" + obj2->type->name + "出来ない").c_str());
    }
    
    langObject Object::inc(langObject obj1)
    {
        switch (obj1->type->TypeEnum)
        {
        case PreType::_Int:
            return newInt(Int::toInt(obj1) + 1);
        //case PreType::_ClassObject:
        //    OPERA2ARG("inc")
        }
        throw langRuntimeException((std::string(obj1->type->name) + "++出来ない").c_str());
        //変換不可
    }

}
