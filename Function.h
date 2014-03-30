#ifndef __LANG_FUNCTION_H__//#pragma once
#define __LANG_FUNCTION_H__
#include "lang.h"
#ifdef CPP11
#include <thread>
#endif
#include "scope.h"
#include "GC.h"
#include <vector>
//#include "Object.h"
namespace lang
{
	enum ENUMCLASS functype : unsigned char
	{
		normal = 0,
		lambda = 1,
		var_arg = 2,
		overload = 4,
	};
	extern
#ifdef _MSC_VER
		__declspec(thread)
#else
		__thread
#endif
		std::vector<langFunction>* stacktrace;
	typedef std::pair<std::string, std::string> FunctionArgUnWrap;
	typedef std::vector<FunctionArgUnWrap> FunctionArg;
	const std::string const emptystr;
	class Function : public Object
	{
	public:
		//関数に関連付けられたstd::thread
#ifdef CPP11
		std::thread* thread;
#endif
		bool working;
		//std::string* getString();
		//void setString(std::string* i);
		std::string name;
		//type,name
		FunctionArg* argList;
		int index;
		scope* scope;
		lang::scope* thisscope;
		Function(std::string name, FunctionArg* argList, lang::scope* scope, int index);
		Function(std::string name, FunctionArg& argList, lang::scope* scope, int index);
		Function(Function* f, lang::scope* this_scope);
		virtual ~Function(void);
		virtual std::string toString();
		virtual langObject call(std::vector<langObject>* argList);
		virtual langObject ctorcall(std::vector<langObject>* argList);
		functype Ftype;
		Function();
		inline bool islambda()
		{
			return (static_cast<unsigned char>(this->Ftype) & static_cast<unsigned char>(functype::lambda));
		}
		inline bool isvar_arg()
		{
			return (static_cast<unsigned char>(this->Ftype) & static_cast<unsigned char>(functype::var_arg));
		}
		inline bool isoverload()
		{
			return (static_cast<unsigned char>(this->Ftype) & static_cast<unsigned char>(functype::overload));
		}
	};
	//ﾓwｯﾎwｫ
	class Lambda :public Function
	{
	public:
		int endindex;
		virtual langObject call(std::vector<langObject>* argList);
		Lambda(std::string name, FunctionArg* argList, lang::scope* scope, int index, int endindex, bool isexp);
		Lambda(std::string name, FunctionArg& argList, lang::scope* scope, int index, int endindex, bool isexp);
		Lambda(Lambda* f, lang::scope* this_scope);
		bool NoExpLambda;
		int startindex;
	};
	extern langObject(*FuncTable[])(langObject, std::vector<langObject>);
	class SpecialFunction : public Function
	{
	public:
		int id;
		SpecialFunction(int id_) : Function("built-in function", nullptr, nullptr, -1), id(id_)
		{
			this->type->TypeEnum = _BuiltFunc;
			this->type->name = "built-in function";
			//lang::gc->uncontroll(this);
			//delete this->type;
		}/*
		 SpecialFunction(langObject _t,SpecialFunction* sp) : id(sp->id),_this(_t) ,Function("built-in function",nullptr,nullptr,-1)
		 {
		 }*/
		virtual langObject call(langObject _this, std::vector<langObject>* argList)
		{
			return (*FuncTable[this->id])(_this, *argList);
		}
		virtual ~SpecialFunction(void);
	};
	class OverloadFunction : public Function
	{
	public:
		std::vector<langFunction> functions;
		OverloadFunction(langFunction, langFunction);
		virtual langObject call(std::vector<langObject>* argList);
		virtual langObject ctorcall(std::vector<langObject>* argList);
	};
	//typedef std::shared_ptr<Function> langFunction;
	class Property : public Object
	{
	private:
		langFunction getter;
		langFunction setter;
	public:
		Property(langFunction Getter, langFunction Setter);
		langObject Get();
		langObject Set(langObject);
	};
}
#endif