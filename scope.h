//#pragma once
#ifndef __SCOPE_H__
#define __SCOPE_H__
#include "lang.h"
#include <vector>
#include "parseObj.h"
#include <map>
#include <memory>
#include "variable.h"
//#include "GC.h"
//#include "Function.h"
//#include "Object.h"
//#define std::shared_ptr<Function> Function
namespace lang
{
	class Function;
	///typedef std::shared_ptr<Function> langFunction;
	/*class RuntimeObject
	{
	public:
	Object* object;
	int index;
	RuntimeObject(Object* obj,int i)
	{
	this->object = obj;
	this->index = i;
	}
	};*/
	namespace en
	{
		enum scopeStatus
		{
			none, iden, var, _for, _if, _while, _foreach
		};
		enum returnStatus
		{
			none_, _return, _break, _continue
		};
		enum scopeType
		{
			_none_, _function, for_, ctor,
		};
	}

	enum ENUMCLASS evals
	{
		isbinaryoperation = 1,
		arrayset = 2,
		mage = 4,
		dmmy2 = 8,
	};
#define isClass() _this
	//std::map<std::string,langObject>
	class scope
	{
	private:
		int refcount;
	public:
		void refinc();
		void refdec();
		void del();
		std::vector<parseObj*>& parsers;
		int startIndex;
		int index;
		bool iscatcher;
		en::scopeType type;
		en::returnStatus status;
		variable variable;
		scope(std::vector<parseObj*>& v);
		scope(std::vector<parseObj*>& v, scope* parent, langClassObject _this);
		void statement(void);
		langObject run(void);
		~scope(void);
		int parentSkip(int index);
		int bracketSkip(int index);
		int blockSkip(int index, int j = 0);
		langObject eval(langObject object, int& index, int opera = 17, evals ev = (evals)0, int unoopera = 17);
		langFunction anonymousFunction(int& index);
		langClassObject _this;
		scope *parent;
	};
#define DEFINEDSCPEVAR(scope, name) (scope->variable.definedVar(name,scope))
	langObject BuidInFunction(std::string name, std::vector<langObject> arg);
}
#endif
