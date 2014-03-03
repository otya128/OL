#include <memory>
#pragma once
namespace lang
{
    extern bool ahogc,parserresult, leakcheck, pause;
    extern bool gc_view;
    extern int error_level;
    extern bool running;
#define v（’ω’）v //たしかに~~~~~
#define var_ auto
#define in_ :
#define foreach_ for
#define is ->type->TypeEnum==
    class Object;
    class Int;
    class String;
    class Function;
    class Class;
    class ClassObject;
    class SpecialFunction;
    class Array;
#define GCENABLE
    //参照ポインタGC
#ifdef GCENABLE
    class GC;
    typedef GC がべこれ;
    extern がべこれ* gc;
    typedef Object* langObject;
    typedef Int* langInt;
    typedef String* langString;
    typedef Class* langClass;
    typedef ClassObject* langClassObject;
    typedef Function* langFunction;
    typedef Array* langArray;
#define newObject(a) new lang::Object(a)
#define newInt(a) new lang::Int(a)
#define newString(a) new lang::String(a)
#define newFunction(a,a1,a2,a3) new lang::Function(a,a1,a2,a3)
#define newClass(a,a1,a2,a3) new lang::Class(a,a1,a2,a3)
#define newClassObject(a) new lang::ClassObject(a)
#define newArray(a) new lang::Array(a);
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
}
