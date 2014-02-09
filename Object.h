
#pragma once
#ifndef newObject
#include <memory>
#include <map>
#include <vector>
//#include "GC.h"
//#include "scope.h"

namespace lang
{
//#define newObject(a) std::make_shared<Object>(a)

class GC;
class Object;
class Int;
class String;
class Function;
#define GCENABLE 1
#ifdef GCENABLE
typedef Object* langObject;
typedef Int* langInt;
typedef String* langString;
#define newObject(a) new Object(a)
#endif
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
    static langObject plus(langObject obj1,langObject obj2);
    static langObject multiply(langObject obj1,langObject obj2);
    static langObject greater(langObject obj1,langObject obj2);
    static langObject less(langObject obj1,langObject obj2);
    static langObject greaterEqual(langObject obj1,langObject obj2);
    static langObject lessEqual(langObject obj1,langObject obj2);
    static langObject modulo(langObject obj1,langObject obj2);
    static langObject equal(langObject obj1,langObject obj2);
};

//typedef langObject langObject;
class Int : public Object
{
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
	std::string* getString();
	void setString(std::string* i);
	String(std::string* ptr);
	~String(void);
    virtual std::string toString();
};

//typedef std::shared_ptr<String> langString;


        langObject const NULLOBJECT = newObject();//std::make_shared<Object>();

}
#endif
