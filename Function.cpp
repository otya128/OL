#include "stdafx.h"
#include "Function.h"
#include <sstream>
#include "langException.h"
namespace lang
{

#ifdef _MSC_VER
	__declspec(thread)
#else
	__thread
#endif
		std::vector<langFunction>* stacktrace;
	void FunctionArgThrow(Function *func, std::vector<langObject>* argList)
	{
		std::stringstream ss;
		ss << "Function:" << func->name << "の引数の数は" << func->argList->size() << "で" << argList->size() << "個呼び出しています。";
		for (int i = 0; i < func->argList->size(); i++)
		{
			if (i)
			{
				ss << ',' << (*func->argList)[i];
			}
			else
			{
				ss << (*func->argList)[i];
			}
		}
		ss << std::endl;
		for (int i = 0; i < argList->size(); i++)
		{
			if (i)
			{
				ss << ',' << (*argList)[i]->toString();
			}
			else
			{
				ss << (*argList)[i]->toString();
			}
		}
		throw langRuntimeException(ss.str().c_str());
	}
	Function::Function(std::string name, std::vector<std::string>& argList, lang::scope* scope, int index)
	{
		this->is_lambda = false;
		this->working = false;
#ifdef CPP11
		this->thread = nullptr;
#endif
		this->type = new Type(PreType::_Function);
		this->name = name;
		this->argList = new std::vector<std::string>(argList);
		this->thisscope = nullptr;
		this->scope = scope;//std::make_shared<lang::scope>(*scope);
		if (this->scope != nullptr)scope->refinc();
		this->index = index;
	}
	Function::Function(std::string name, std::vector<std::string>* argList, lang::scope* scope, int index)
	{
		this->is_lambda = false;
		this->working = false;
#ifdef CPP11
		this->thread = nullptr;
#endif
		this->type = new Type(PreType::_Function);
		this->name = name;
		this->argList = argList;
		this->thisscope = nullptr;
		this->scope = scope;//std::make_shared<lang::scope>(*scope);
		if (this->scope != nullptr)scope->refinc();
		this->index = index;
	}
	Function::Function(Function* f, lang::scope* this_scope)
	{
		this->is_lambda = false;
		this->working = false;
#ifdef CPP11
		this->thread = nullptr;
#endif
		//if(f is _Function){}
		this->type = new Type(f->type->TypeEnum, (char*)f->type->name);
		this->name = f->name;
		this->argList = new std::vector<std::string>(*f->argList);
		this->scope = f->scope;
		this->index = f->index;
		this->thisscope = this_scope;
		if (this->thisscope != nullptr)this_scope->refinc();
		if (this->scope != nullptr)this->scope->refinc();
		this->ptr = nullptr;
	}
	Function::~Function(void)
	{
		//if(thread)thread->detach();
#ifdef CPP11
		delete thread;
#endif

		if (this->scope != nullptr)this->scope->refdec();
		if (this->thisscope != nullptr)this->thisscope->refdec();
		//this->scope = nullptr;
		//this->ptr = new int(1);

		delete this->argList;
		//if(name != nullptr)
#if _DEBUG
		if (gc_view)
		{
			std::cout << "がべこれ中..." << name << this << std::endl;
			//delete this->name;
		}
#endif
		// else std::cout<<"がべこれ中...null"<<this<<std::endl;
	}
	std::string Function::toString(void)
	{
		return "function:" + name;
	}
	langObject Function::call(std::vector<langObject>* argList)
	{
		lang::stacktrace->push_back(this);
		working = true;
		auto sc = new lang::scope(this->scope->parsers, this->scope/*.get()*/, this->scope->_this);
		if (name.size() && name.at(0) == '#')sc->variable.add(name.substr(1), this);//new Function(this,sc));
		sc->type = en::scopeType::_function;
		sc->startIndex = this->index + 1;//+1しんあいとreturnが無い関数でreturnされなくなる
		if (this->argList->size() != argList->size())
		{
			FunctionArgThrow(this, argList);
		}
		for (int i = 0; i < this->argList->size(); i++)
		{
			sc->variable.add((*this->argList)[i], (*argList)[i]);
		}
		langObject buf;
		buf = sc->run();
		sc->del();
		working = false;
		lang::stacktrace->pop_back();
		return /*std::shared_ptr<Object>*/(buf);
	}
	Lambda::Lambda(std::string name, std::vector<std::string>* argList, lang::scope* scope, int index, int endindex, bool isexp)
		: Function(name, argList, scope, index)
	{
		this->is_lambda = true;
		this->type->name = this->name.c_str();
		this->endindex = endindex;
		this->NoExpLambda = isexp;
		//this->type->TypeEnum = _Lambda;
	}
	Lambda::Lambda(std::string name, std::vector<std::string>& argList, lang::scope* scope, int index, int endindex, bool isexp)
		: Function(name, argList, scope, index)
	{
		this->is_lambda = true;
		this->type->name = this->name.c_str();
		this->endindex = endindex;
		this->NoExpLambda = isexp;
		//this->type->TypeEnum = _Lambda;
	}
	Lambda::Lambda(Lambda* f, lang::scope* this_scope) : Function(f, this_scope)
	{
		this->is_lambda = true;
		this->type->name = this->name.c_str();
		this->endindex = f->endindex;
		this->NoExpLambda = f->NoExpLambda;
		this->scope = this->thisscope;
		//this->type->TypeEnum = _Lambda;
	}
	langObject Lambda::call(std::vector<langObject>* argList)
	{
		lang::stacktrace->push_back(this);
		if (this->NoExpLambda) return Function::call(argList);
		working = true;
		auto sc = new lang::scope(this->scope->parsers, this->scope/*.get()*/, this->scope->_this);
		if (name.size() && name.at(0) == '#')sc->variable.add(name.substr(1), this);//new Function(this,sc));
		sc->type = en::scopeType::_function;
		sc->startIndex = this->index;// + 1;//+1しんあいとreturnが無い関数でreturnされなくなる
		if (this->argList->size() != argList->size())
			FunctionArgThrow(this, argList);
		for (int i = 0; i < this->argList->size(); i++)
		{
			sc->variable.add((*this->argList)[i], (*argList)[i]);
		}
		langObject buf;
		buf = sc->eval(NULLOBJECT, sc->startIndex);//run();
		sc->del();
		working = false;
		lang::stacktrace->pop_back();
		return /*std::shared_ptr<Object>*/(buf);
	}
	langObject Function::ctorcall(std::vector<langObject>* argList)
	{
		lang::stacktrace->push_back(this);
		auto sc = new lang::scope(this->scope->parsers, this->scope, this->scope->_this);
		sc->type = en::scopeType::ctor;
		sc->startIndex = this->index + 1;//+1しんあいとreturnが無い関数でreturnされなくなる
		if (this->argList->size() != argList->size())
			FunctionArgThrow(this, argList);
		for (int i = 0; i < this->argList->size(); i++) sc->variable.add((*this->argList)[i], (*argList)[i]);
		auto buf = sc->run();
		sc->del();
		lang::stacktrace->pop_back();
		return buf;
	}
	langFunction scope::anonymousFunction(int& index)
	{
		std::stringstream str;
		str << "anonymousFunction" << index;
		//str=;
		std::string name = str.str();
		int funcRead = 2;
		std::vector<std::string>* argList = new std::vector<std::string>();

		langFunction func = /*std::make_shared<Function>*/new Function(name, nullptr, this, index + 1);
		for (int i = index + 1; i < this->parsers.size(); i++)
		{
			auto token = this->parsers[i];
			if (!funcRead)break;
			switch (funcRead)
			{
				case 2://(
					//名前あり匿名関数
					if (token->pEnum == parserEnum::identifier)
					{
						func->name = "#" + *token->name;
						continue;
					}
					if (token->pEnum == parserEnum::leftparent)funcRead++; else funcRead = 0;
					if (token->pEnum == parserEnum::rightparent)funcRead = 6;
					//引数省略匿名関数
					if (token->pEnum == parserEnum::blockstart) funcRead = 6;
					break;
				case 3://type 
					if (token->pEnum == parserEnum::identifier)funcRead++; else funcRead = 0;
					if (token->pEnum == parserEnum::rightparent)funcRead = 6;
					break;
				case 4://name
					if (token->pEnum == parserEnum::identifier)
					{
						argList->push_back(*token->name);
						funcRead++;
					}
					else
					if (token->pEnum == parserEnum::comma || token->pEnum == parserEnum::rightparent)
					{
						funcRead--;
						argList->push_back(*((this->parsers[i - 1])->name));
						if (token->pEnum == parserEnum::rightparent)funcRead = 6;
					}
					else
						funcRead = 0;
					break;
				case 5://, or )
					if (token->pEnum == parserEnum::comma)funcRead -= 2;
					else if (token->pEnum == parserEnum::rightparent)funcRead++;
					else funcRead = 0;
					break;
				case 6://{
					funcRead = 0;
					func->argList = argList;
					func->index = i;
					//func = new Function(name, argList, this,i);
					//this->runner->variable.add(*funcName,std::make_shared<langFunction>(*funcName,argList,this->runner));
					break;
			}
		}
		if (!func->argList)throw langRuntimeException("syntax error anonymousFunction");
		this->refinc();
		index = this->blockSkip(func->index);
		return /*std::shared_ptr<Function>*/(func);
	}
	langObject Object_ToString(langObject obj, std::vector<langObject> argList)
	{
		return newString(obj->toString());
	}
	langObject String_Substring(langObject obj, std::vector<langObject> argList)
	{
		if (argList.size() == 1 && obj is _String)
		{
			auto bug = (langString)obj;
			return newString(bug->getString()->substr(Int::toInt(argList[0])));
		}
		else
		if (argList.size() == 2 && obj is _String)
		{
			auto bug = (langString)obj;
			return newString(bug->getString()->substr(Int::toInt(argList[0]), Int::toInt(argList[1])));
		}
		throw lang::langRuntimeException("引数の数が一致しません[String.Substring(,)]");
	}
	typedef langObject(*BuiltFunc)(std::vector<langObject>);
	std::map<std::string, BuiltFunc>* BuiltFunction = new std::map<std::string, BuiltFunc>;
	langObject(*FuncTable[])(langObject, std::vector<langObject>) =
	{
		&Object_ToString,
		&String_Substring,
	};
	SpecialFunction::~SpecialFunction(void)
	{
	}
}
