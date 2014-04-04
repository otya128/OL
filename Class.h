#ifndef __LANG_CLASS_H__//#pragma once
#define __LANG_CLASS_H__
#include "lang.h"
#include "Object.h"
#include "variable.h"
#include "langException.h"
namespace lang
{
	class scope;
#define MEMBERTYPEITEM(x,y,z) membertypeitem(x, std::pair<langObject, qualifier>(y,z)) 
	typedef std::pair<std::string, std::pair<langObject, qualifier> > membertypeitem;
	typedef std::vector<membertypeitem> membertype_;
	typedef membertype_* membertype;
	class Class : public Object
	{
	protected:
		Class(Class* clas);
	public:
		membertype member;
		langFunction finalize;
		Class(){}
		Class(std::string name, int index, membertype member, scope* scope, membertype staticmember);
		virtual ~Class(void);
		//名前
		std::string name;
		lang::scope* scope;
		int index;
		virtual std::string toString();
		//スコープ
		lang::scope* thisscope;
		Class* base;
		virtual bool trygetMember(const char* name, langObject& obj, lang::scope *access);
		virtual langObject getMember(const char* name, lang::scope *access);
		virtual langObject setMember(const char* name, langObject obj, lang::scope *access);
		virtual bool trygetMember(std::string& name, langObject& obj, lang::scope *access);
		virtual langObject getMember(std::string& name, lang::scope *access);
		virtual langObject setMember(std::string& name, langObject obj, lang::scope *access);
		virtual langClassObject CreateObject(Class* type);
	};
	class ClassObject : public Class
	{
	public:
		Class* staticClass;
		virtual std::string toString(void);
		ClassObject(Class* type);
		~ClassObject(void);
	};
	class ArrayBufferClass : public Class
	{
	public:
		ArrayBufferClass(lang::scope *scopec);
		virtual langClassObject CreateObject(Class* type);
	};
	class ArrayBufferClassObject : public ClassObject
	{
	public:
		size_t size;
		bool nonunique;
		ArrayBufferClassObject(void);
		~ArrayBufferClassObject(void);
	};
	size_t ArrayBufferGetSize(langClassObject this_);
	size_t ArrayBufferSetSize(langClassObject this_, size_t);
	void* ArrayBufferGetPointer(langClassObject this_);
	void ArrayBufferSetPointer(langClassObject this_, void*);
	bool ArrayBufferSetNonUnique(langClassObject this_, bool);
}
#endif
