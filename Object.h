#pragma once
#include <memory>

namespace lang
{

enum PreType
{
    _Object,_Int,_String,_Char,_Double,_Array,_Class
};
class Type
{
public:
    const char* name;
    PreType TypeEnum;
    Type(char* name)
    {
        this->name=name;
    }
    Type(PreType enu)
    {
        this->TypeEnum = enu;
        switch (enu)
        {
        case _Object:
            this->name = "Object";
            break;
        case _Int:
            this->name = "Int";
            break;
        case _String:
            this->name = "String";
            break;
        case _Char:
            this->name = "Char";
            break;
        case _Double:
            this->name = "Double";
            break;
        case _Array:
            this->name = "Array";
            break;
        case _Class:
            this->name = "Class";
            break;
        default:
            break;
        }
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

}
