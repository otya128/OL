#include <memory>
#pragma once
namespace lang
{
extern bool gc_view;
extern int error_level;
#define v（’ω’）v //たしかに~~~~~
#define var_ auto
#define in_ :
#define foreach_ for
class Object;
class Int;
class String;
class Function;
class Class;
class ClassObject;
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
#define newObject(a) new Object(a)
#define newInt(a) new Int(a)
#define newString(a) new String(a)
#define newFunction(a,a1,a2,a3) new Function(a,a1,a2,a3)
#define newClass(a,a1,a2,a3) new Class(a,a1,a2,a3)
#define newClassObject(a) new ClassObject(a)
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
