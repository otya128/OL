#ifndef __OBJECT_H__
#define __OBJECT_H__
//#pragma once
#include <memory>
#include <map>
#include <vector>
//#include "GC.h"
//#include "scope.h"
#include "lang.h"
namespace lang
{
    //#define newObject(a) std::make_shared<Object>(a)
    //int objectalloc_count = 0;
	enum PreType
	{
		_Object, _Int, _String, _Char, _WChar, _Double, _Array, _Class, _Function, _ClassObject, _BuiltFunc, _Type, _Property
	};
#pragma once
	static const char* PreTypeName[] = { "Object", "Int", "String", "Char", "WChar", "Double", "Array", "Class", "Function", "ClassObject", "BuiltFunc", "Type", "Property"};

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
    extern Type* ObjectTypeClass;
	class Object
	{
	protected:
		void* ptr;
	public:
		Type* type;
		void* getPointer();
		virtual void setPointer(void* ptr);
		virtual std::string toString();
		virtual langObject getMember(const char* name);
		virtual langObject setMember(const char* name, langObject obj);
		virtual langObject getMember(std::string& name);
		virtual langObject setMember(std::string& name, langObject obj);
		Object(void* ptr);
		Object(void);
		virtual ~Object(void);
		static langObject inc(langObject obj1);
		static langObject dec(langObject obj1);
		static langObject bracket(langObject obj1, langObject obj2);
		static langObject bracketequal(langObject obj1, langObject obj2, langObject obj3);
		static langObject bracket(langObject obj1, std::vector<langObject> obj2);
		static langObject bracketequal(langObject obj1, std::vector<langObject> obj2);
		static langObject plus(langObject obj1, langObject obj2);
		static langObject minus(langObject obj1, langObject obj2);
		static langObject unaryplus(langObject obj1);
		static langObject unaryminus(langObject obj1);
		static langObject multiply(langObject obj1, langObject obj2);
		static langObject division(langObject obj1, langObject obj2);
		static langObject greater(langObject obj1, langObject obj2);
		static langObject less(langObject obj1, langObject obj2);
		static langObject greaterEqual(langObject obj1, langObject obj2);
		static langObject lessEqual(langObject obj1, langObject obj2);
		static langObject modulo(langObject obj1, langObject obj2);
		static langObject equal(langObject obj1, langObject obj2);
		static langObject leftShift(langObject obj1, langObject obj2);
		static langObject rightShift(langObject obj1, langObject obj2);
		static langObject _is(langObject obj1, langObject obj2);
		static langObject as(langObject obj1, langObject obj2);


		//�r�b�g���Z�n
		static langObject _and(langObject obj1, langObject obj2);
		static langObject _or(langObject obj1, langObject obj2);
		static langObject _not(langObject obj1);
		static langObject _xor(langObject obj1, langObject obj2);
		static langObject logicnot(langObject obj1);
		static langObject logicand(langObject obj1, langObject obj2);
		static langObject logicor(langObject obj1, langObject obj2);
		//?=�n
		static langObject plusEqual(langObject obj1, langObject obj2);
		static langObject minusEqual(langObject obj1, langObject obj2);
		static langObject multiplyEqual(langObject obj1, langObject obj2);
		static langObject divisionEqual(langObject obj1, langObject obj2);
		static langObject moduloEqual(langObject obj1, langObject obj2);
		static langObject andEqual(langObject obj1, langObject obj2);
		static langObject orEqual(langObject obj1, langObject obj2);
		static langObject xorEqual(langObject obj1, langObject obj2);
		static langObject leftShiftEqual(langObject obj1, langObject obj2);
		static langObject rightShiftEqual(langObject obj1, langObject obj2);

		static langObject pow(langObject obj1, langObject obj2);
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
	class ObjectType : public Object
	{
	public:
		ObjectType();
		Type TypeClass;
		virtual langObject create(std::vector<langObject> &arg);
	};
	class IntType : public Object
	{
	public:
		IntType();
		Type TypeClass;
		virtual langObject create(std::vector<langObject> &arg);
	};
	class StringType : public Object
	{
	public:
		StringType();
		Type TypeClass;
		virtual langObject create(std::vector<langObject> &arg);
	};
	class DoubleType : public Object
	{
	public:
		DoubleType();
		Type TypeClass;
		virtual langObject create(std::vector<langObject> &arg);
	};
	class CharType : public Object
	{
	public:
		CharType();
		Type TypeClass;
		virtual langObject create(std::vector<langObject> &arg);
	};
	class WCharType : public Object
	{
	public:
		WCharType();
		Type TypeClass;
		virtual langObject create(std::vector<langObject> &arg);
	};
	class ArrayType : public Object
	{
	public:
		ArrayType();
		Type TypeClass;
		virtual langObject create(std::vector<langObject> &arg);
	};
    //typedef std::shared_ptr<Int> langInt;
    class String : public Object
    {
    public:
		virtual langObject getMember(std::string& name);
		virtual langObject getMember(const char* name);
        std::string* getString();
        void setString(std::string* i);
		String(std::string* i);
		String(std::string &i);
		String(std::string i);
		String(char* i);
        ~String(void);
		virtual std::string toString();
		void* getPointer();
    };
    
    class Char : public Object
    {
    public:
        char getChar();
        void setChar(char i);
        Char(char ptr);
        ~Char(void);
        static char toChar(langObject obj);
        virtual std::string toString();
    private:
        char chr;
    };
    typedef wchar_t wchar;
    class WChar : public Object
    {
    public:
        wchar getWChar();
        void setWChar(wchar i);
        WChar(wchar ptr);
        ~WChar(void);
        static wchar toWChar(langObject obj);
        virtual std::string toString();
    private:
        wchar chr;
	};
	class Double : public Object
	{
	public:
		double getDouble();
		void setDouble(double i);
		Double(double ptr);
		~Double(void);
		static double toDouble(langObject obj);
		virtual std::string toString();
	private:
		double dbl;
	};
    //typedef std::shared_ptr<String> langString;


    extern langObject NULLOBJECT;// = newObject(nullptr);//std::make_shared<Object>();
    extern langObject FALSEOBJECT;
    extern langObject TRUEOBJECT;

}
#endif
