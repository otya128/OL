#ifndef __LANG_H__
#define __LANG_H__
#include <memory>
//#pragma once
//#define OL_GTK
//#define CPP11 1
#define OL_GTK 1
#define nullptr 0 
int main(int, char**);
#define ENDFOREACH }
#ifdef CPP11
#define ENUMCLASS class
#define FOREACH(VAR,COR) for(auto VAR : COR){
#else
#define ENUMCLASS 
#define FOREACH(type,VAR,type2,COR) for(type it = (COR).begin(); it != (COR).end(); ++it ){type2 VAR = *it;
#endif

namespace lang
{
	extern bool ahogc, parserresult, leakcheck, pause, prompt;
	extern bool gc_view;
	extern int error_level;
	extern bool running;
#ifdef _MSC_VER
#define v___  
#define __v(x)  static_assert(x == 'ω', "KANI ERROR");
#define v_____v(x)  static_assert(x == 'ω', "KANI ERROR");
#else
#define v___  
#define __v(x) 
#define v_____v(x) 
#endif
	//#define v（’ω’）v //たしかに~~~~~
#define var_ auto
#define in_ :
#define foreach_ for
#define is ->type->TypeEnum==
	class Object;
	class Int;
	class String;
	class Function;
	class Lambda;
	class Class;
	class ClassObject;
	class SpecialFunction;
	class Array;
	class Char;
	class Double;
	class Property;
#define GCENABLE
	//参照ポインタGC
#ifdef GCENABLE
	class GC;
	typedef GC gabekore;
	extern gabekore* gc;
	typedef Object* langObject;
	typedef Int* langInt;
	typedef String* langString;
	typedef Class* langClass;
	typedef ClassObject* langClassObject;
	typedef Function* langFunction;
	typedef Lambda* langLambda;
	typedef Array* langArray;
	typedef Char* langChar;
	typedef Double* langDouble;
#define newObject(a) new lang::Object(a)
#define newInt(a) new lang::Int(a)
#define newString(a) new lang::String(a)
#define newFunction(a,a1,a2,a3) new lang::Function(a,a1,a2,a3)
#define newClass(a,a1,a2,a3,a4) new lang::Class(a,a1,a2,a3,a4)
#define newClassObject(a) new lang::ClassObject(a)
#define newArray(a) new lang::Array(a);
#define newChar(a) new lang::Char(a);
#define newDouble(a) new lang::Double(a);
#endif
#ifndef GCENABLE
	typedef std::shared_ptr<Object> langObject;
	typedef std::shared_ptr<Int> langInt;
	typedef std::shared_ptr<String> langString;
	typedef std::shared_ptr<Function> langFunction;
#define newObject(a) std::make_shared<Object>(a)
#define newInt(a) std::make_shared<Int>(a)
#define newString(a) std::make_shared<String>(a)
#define newFunction(a,a1,a2,a3) std::make_shared<Function>(a,a1,a2,a3)
#endif
	class ObjectType;
	class IntType;
	class StringType;
	class DoubleType;
	class CharType;
	class WCharType;
	class ArrayType;
	extern ArrayType* ArrayTypeObject;
	extern ObjectType* ObjectTypeObject;
}
#endif
