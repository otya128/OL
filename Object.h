
#pragma once
#include <memory>
#include <map>
#include <vector>
//#include "GC.h"
//#include "scope.h"
#include "lang.h"
namespace lang
{
    //#define newObject(a) std::make_shared<Object>(a)

    enum PreType
    {
        _Object,_Int,_String,_Char,_Double,_Array,_Class,_Function,_ClassObject,_BuiltFunc
    };
#pragma once
    static const char* PreTypeName[] = {"Object","Int","String","Char","Double","Array","Class","Function","ClassObject","BuiltFunc"};

    class Type
    {
    public:
        static std::map<PreType,Type*> TypePool;
        const char* name;
        PreType TypeEnum;
        Type(char* name)
        {
            this->name=name;
        }
        Type(PreType enu)
        {
            this->TypeEnum = enu;
            this->name = PreTypeName[enu];
            //case _Class:
            //    this->name = "Class";
            //    break;
        }
        Type(PreType enu,char *name)
        {
            this->TypeEnum = enu;
            this->name = name;
        }
        ~Type(void)
        {
            //delete this->name;
        }
    };
    extern Type* ObjectType;
    class Object
    {
    protected:
        void* ptr;
    public:
        Type* type;
        void* getPointer();
        void setPointer(void* ptr);
        virtual std::string toString();
        virtual langObject getMember(std::string& name);
        Object(void* ptr);
        Object(void);
        virtual ~Object(void);
        static langObject inc(langObject obj1);
        static langObject plus(langObject obj1,langObject obj2);
        static langObject multiply(langObject obj1,langObject obj2);
        static langObject greater(langObject obj1,langObject obj2);
        static langObject less(langObject obj1,langObject obj2);
        static langObject greaterEqual(langObject obj1,langObject obj2);
        static langObject lessEqual(langObject obj1,langObject obj2);
        static langObject modulo(langObject obj1,langObject obj2);
        static langObject equal(langObject obj1,langObject obj2);
        static langObject leftShift(langObject obj1,langObject obj2);
        static langObject rightShift(langObject obj1,langObject obj2);
    };
    extern SpecialFunction* object_tostr;
    extern SpecialFunction* string_substr;
    //typedef langObject langObject;
    class Int : public Object
    {
    #if _DEBUG
        int debuggg;
    #endif
    public:
        int getInt();
        void setInt(int i);
        Int(int ptr);
        ~Int(void);
        static int toInt(langObject obj);
        virtual std::string toString();
    };
    //typedef std::shared_ptr<Int> langInt;
    class String : public Object
    {
    public:
        virtual langObject getMember(std::string& name);
        std::string* getString();
        void setString(std::string* i);
        String(std::string* ptr);
        ~String(void);
        virtual std::string toString();
    };

    //typedef std::shared_ptr<String> langString;


    extern langObject NULLOBJECT;// = newObject(nullptr);//std::make_shared<Object>();

}
