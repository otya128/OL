
#pragma once
#ifndef newObject
#include <memory>
#include <map>
#include <vector>
//#include "scope.h"

namespace lang
{
enum PreType
{
    _Object,_Int,_String,_Char,_Double,_Array,_Class,_Function
};
#pragma once
static const char* PreTypeName[] = {"Object","Int","String","Char","Double","Array","Class","Function"};

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
    ~Type(void)
    {
        //delete this->name;
    }
};
class Object
{
protected:
	void* ptr;
public:
    Type* type;
	void* getPointer();
	void setPointer(void* ptr);
    virtual std::string toString();
	Object(void* ptr);
    Object(void);
	~Object(void);
    static std::shared_ptr<Object> plus(std::shared_ptr<Object> obj1,std::shared_ptr<Object> obj2);
    static std::shared_ptr<Object> multiply(std::shared_ptr<Object> obj1,std::shared_ptr<Object> obj2);
    static std::shared_ptr<Object> greater(std::shared_ptr<Object> obj1,std::shared_ptr<Object> obj2);
    static std::shared_ptr<Object> less(std::shared_ptr<Object> obj1,std::shared_ptr<Object> obj2);
    static std::shared_ptr<Object> greaterEqual(std::shared_ptr<Object> obj1,std::shared_ptr<Object> obj2);
    static std::shared_ptr<Object> lessEqual(std::shared_ptr<Object> obj1,std::shared_ptr<Object> obj2);
    static std::shared_ptr<Object> modulo(std::shared_ptr<Object> obj1,std::shared_ptr<Object> obj2);
    static std::shared_ptr<Object> equal(std::shared_ptr<Object> obj1,std::shared_ptr<Object> obj2);
};

typedef std::shared_ptr<Object> langObject;
#define newObject(a) std::make_shared<Object>(a)
class Int : public Object
{
public:
	int getInt();
	void setInt(int i);
	Int(int ptr);
	~Int(void);
    static int toInt(std::shared_ptr<Object> obj);
    virtual std::string toString();
};
typedef std::shared_ptr<Int> langInt;
class String : public Object
{
public:
	std::string* getString();
	void setString(std::string* i);
	String(std::string* ptr);
	~String(void);
    virtual std::string toString();
};

typedef std::shared_ptr<String> langString;

        langObject const NULLOBJECT = newObject();//std::make_shared<Object>();

}
#endif
