#include "stdafx.h"
//#include "scope.h"
#include "Function.h"
#include <sstream>
#include "langException.h"
#include "Class.h"
#define INT_MAX       2147483647    /* maximum (signed) int value */
namespace lang
{
  #define parserEnum lang
#ifdef _MSC_VER
	__declspec(thread)
#else
	__thread
#endif
		std::vector<langFunction>* stacktrace;
	void FunctionArgThrow(Function *func, std::vector<langObject>* argList)
	{
		std::stringstream ss;
		ss << "Function:" << func->name << "�̈����̐���" << func->argList->size() << "��" << argList->size() << "�Ăяo���Ă��܂��B";
		for (int i = 0; i < func->argList->size(); i++)
		{
			if (i)
			{
				ss << ',' << (*func->argList)[i].second;
			}
			else
			{
				ss << (*func->argList)[i].second;
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
	void FunctionArgThrow(OverloadFunction *func, std::vector<langObject>* argList)
	{
		std::stringstream ss;
		ss << "Function:" << func->name << "�̈��v�����֐����������܂���";
		for (int j = 0; j < func->functions.size(); j++)
		{
			for (int i = 0; i < func->functions[j]->argList->size(); i++)
			{
				if (i)
				{
					ss << ',' << (*func->functions[j]->argList)[i].first << ' ' << (*func->functions[j]->argList)[i].second;
				}
				else
				{
					ss << (*func->functions[j]->argList)[i].first << ' ' << (*func->functions[j]->argList)[i].second;
				}
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
	Function::Function(std::string name, std::vector<std::pair<std::string, std::string> >& argList, lang::scope* scope, int index)
	{
		this->Ftype = normal;
		this->working = false;
#ifdef CPP11
		this->thread = nullptr;
#endif
		this->type = new Type(_Function);
		this->name = name;
		this->argList = new FunctionArg(argList);
		this->thisscope = nullptr;
		this->scope = scope;//std::make_shared<lang::scope>(*scope);
		if (this->scope != nullptr)scope->refinc();
		this->index = index;
	}
	Function::Function(std::string name, FunctionArg* argList, lang::scope* scope, int index)
	{
		this->Ftype = normal;
		this->working = false;
#ifdef CPP11
		this->thread = nullptr;
#endif
		this->type = new Type(_Function);
		this->name = name;
		this->argList = argList;
		this->thisscope = nullptr;
		this->scope = scope;//std::make_shared<lang::scope>(*scope);
		if (this->scope != nullptr)scope->refinc();
		this->index = index;
	}
	Function::Function(Function* f, lang::scope* this_scope)
	{
		this->Ftype = f->Ftype;
		this->working = false;
#ifdef CPP11
		this->thread = nullptr;
#endif
		//if(f is _Function){}
		this->type = new Type(f->type->TypeEnum, (char*)f->type->name);
		this->name = f->name;
		this->argList = new FunctionArg(*f->argList);
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
			std::cout << "���ׂ��ꒆ..." << name << this << std::endl;
			//delete this->name;
		}
#endif
		// else std::cout<<"���ׂ��ꒆ...null"<<this<<std::endl;
	}
	std::string Function::toString(void)
	{
		return "function:" + name;
	}
	langObject Function::call(std::vector<langObject>* argList)
	{
		lang::stacktrace->push_back(this);
		working = true;
		lang::scope* sc = new lang::scope(this->scope->parsers, this->scope/*.get()*/, this->scope->_this);
		if (name.size() && name.at(0) == '#')sc->variable.add(name.substr(1), this);//new Function(this,sc));
		sc->type = en::_function;
		sc->startIndex = this->index + 1;//+1���񂠂���return�������֐���return�����Ȃ��Ȃ�
		if (this->argList->size() != argList->size())
		{
			FunctionArgThrow(this, argList);
		}
		for (int i = 0; i < this->argList->size(); i++)
		{
			sc->variable.add((*this->argList)[i].second, (*argList)[i]);
		}
		langObject buf;
		buf = sc->run();
		sc->del();
		working = false;
		lang::stacktrace->pop_back();
		return /*std::shared_ptr<Object>*/(buf);
	}
	Lambda::Lambda(std::string name, FunctionArg* argList, lang::scope* scope, int index, int endindex, bool isexp)
		: Function(name, argList, scope, index)
	{
		this->Ftype = lambda_;
		this->type->name = this->name.c_str();
		this->endindex = endindex;
		this->NoExpLambda = isexp;
		//this->type->TypeEnum = _Lambda;
	}
	Lambda::Lambda(std::string name, FunctionArg& argList, lang::scope* scope, int index, int endindex, bool isexp)
		: Function(name, argList, scope, index)
	{
		this->Ftype = lambda_;
		this->type->name = this->name.c_str();
		this->endindex = endindex;
		this->NoExpLambda = isexp;
		//this->type->TypeEnum = _Lambda;
	}
	Lambda::Lambda(Lambda* f, lang::scope* this_scope) : Function(f, this_scope)
	{
		this->Ftype = f->Ftype;
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
		lang::scope* sc = new lang::scope(this->scope->parsers, this->scope/*.get()*/, this->scope->_this);
		if (name.size() && name.at(0) == '#')sc->variable.add(name.substr(1), this);//new Function(this,sc));
		sc->type = en::_function;
		sc->startIndex = this->index;// + 1;//+1���񂠂���return�������֐���return�����Ȃ��Ȃ�
		if (this->argList->size() != argList->size())
			FunctionArgThrow(this, argList);
		for (int i = 0; i < this->argList->size(); i++)
		{
			sc->variable.add((*this->argList)[i].second, (*argList)[i]);
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
		lang::scope* sc = new lang::scope(this->scope->parsers, this->scope, this->scope->_this);
		sc->type = en::ctor;
		sc->startIndex = this->index + 1;//+1���񂠂���return�������֐���return�����Ȃ��Ȃ�
		if (this->argList->size() != argList->size())
			FunctionArgThrow(this, argList);
		for (int i = 0; i < this->argList->size(); i++) sc->variable.add((*this->argList)[i].second, (*argList)[i]);
		langObject buf = sc->run();
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
		FunctionArg* argList = new FunctionArg();
		std::string type = "";
		langFunction func = /*std::make_shared<Function>*/new Function(name, nullptr, this, index + 1);
		for (int i = index + 1; i < this->parsers.size(); i++)
		{
			parseObj* token = this->parsers[i];
			if (!funcRead)break;
			switch (funcRead)
			{
				case 2://(
					//���O���蓽���֐�
					if (token->pEnum == parserEnum::identifier)
					{
						func->name = "#" + *token->name;
						continue;
					}
					if (token->pEnum == leftparent)funcRead++; else funcRead = 0;
					if (token->pEnum == parserEnum::rightparent)funcRead = 6;
					//�����ȗ������֐�
					if (token->pEnum == parserEnum::blockstart) funcRead = 6;
					break;
				case 3://type 
					if (token->pEnum == parserEnum::identifier)funcRead++; else funcRead = 0;
					if (token->pEnum == parserEnum::rightparent)funcRead = 6;
					break;
				case 4://name
					if (token->pEnum == parserEnum::identifier)
					{
						argList->push_back(FunctionArgUnWrap(emptystr, *token->name));
						funcRead++;
					}
					else
					if (token->pEnum == parserEnum::comma || token->pEnum == parserEnum::rightparent)
					{
						funcRead--;
						argList->push_back(FunctionArgUnWrap(type, *((this->parsers[i - 1])->name)));
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
			langString bug = (langString)obj;
			return newString(bug->getString()->substr(Int::toInt(argList[0])));
		}
		else
		if (argList.size() == 2 && obj is _String)
		{
			langString bug = (langString)obj;
			return newString(bug->getString()->substr(Int::toInt(argList[0]), Int::toInt(argList[1])));
		}
		throw lang::langRuntimeException("�����̐������v���܂���[String.Substring(,)]");
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
	Function::Function()
	{
	}
	OverloadFunction::OverloadFunction(langFunction f1, langFunction f2) : Function()
	{
		this->Ftype = overload;
		this->functions.push_back(f1);
		this->functions.push_back(f2);
		this->working = false;
#ifdef CPP11
		this->thread = nullptr;
#endif
		this->type = new Type(PreType::_Function);
		this->name = f1->name;
		this->argList = nullptr;
		this->thisscope = nullptr;
		this->scope = nullptr;
	}
	int object_distance(langObject m, langObject n);
	langObject OverloadFunction::call(std::vector<langObject>* argList)
	{
		int index = 0;
		int kyorimin = INT_MAX;//int *kyori = (int*)alloca(argList->size() * sizeof(int));
		int kyoricount, kyorin = -1;
		for (int i = 0; i < this->functions.size(); i++)
		{
			if (this->functions[i]->argList->size() == argList->size() ||
				//�ϒ������̏ꍇ
				(this->functions[i]->argList->size() <= argList->size() && this->functions[i]->isvar_arg()))
			{
				kyoricount = 0;
				for (int j = 0; j < argList->size(); j++)
				{
					langObject typo = this->functions[i]->scope->variable((this->functions[i]->argList->at(j).first), this->functions[i]->scope);
					if (typo == NULLOBJECT) typo = ObjectTypeObject;
					int dis = object_distance(argList->at(j), typo);
					if (dis == INT_MAX)
					{
						kyoricount = INT_MAX; break;
					}
					kyoricount += dis;
				}
				if (kyorimin > kyoricount)
				{
					kyorimin = kyoricount;
					kyorin = i;
				}
			}
		}
		if (kyorin == -1)
		{
			FunctionArgThrow(this, argList);
		}
		return this->functions[kyorin]->call(argList);
	}
	langObject OverloadFunction::ctorcall(std::vector<langObject>* argList)
	{
		return this->call(argList);
	}
	Property::Property(langFunction Getter, langFunction Setter, qualifier g, qualifier s)
	{
		this->type = new Type(_Property);
		this->getter = Getter;
		this->setter = Setter;
		this->getqualifier = g;
		this->setqualifier = s;
	}
	langObject Property::Get(variable* v, scope* access)
	{
		if (getter)
		{
			std::vector<langObject> arg;
			qualifier q = this->getqualifier;
			if (q & private_)
			{
				if (!access->_this || v->owner->_this->thisscope != access->_this->thisscope)
				{
					if (access->_this && (q & protected_))
					{
						langClass base = access->_this->base;
						while (base)
						{
							if (v->owner->_this == base)
								return this->getter->call(&arg);
							base = base->base;
						}
						throw_langRuntimeException("protected setter�ɂ̓A�N�Z�X�ł��܂���");
					}
					throw_langRuntimeException("private setter�ɂ̓A�N�Z�X�ł��܂���");
				}
			}
			return this->getter->call(&arg);
		}
		throw langRuntimeException("getter�����݂��܂���");
	}
	langObject Property::Set(langObject value, variable*v, scope* access)
	{
		if (setter)
		{
			std::vector<langObject> arg;
			arg.push_back(value);
			qualifier q = this->setqualifier;
			if (q & private_)
			{
				if (!access->_this || v->owner->_this->thisscope != access->_this->thisscope)
				{
					if (access->_this && (q & protected_))
					{
						langClass base = access->_this->base;
						while (base)
						{
							if (v->owner->_this == base)
							{
								this->setter->call(&arg);
								return value;
							}
							base = base->base;
						}
						throw_langRuntimeException("protected setter�ɂ̓A�N�Z�X�ł��܂���");
					}
					throw_langRuntimeException("private setter�ɂ̓A�N�Z�X�ł��܂���");
				}
			}
			this->setter->call(&arg);
			return value;
		}
		throw langRuntimeException("setter�����݂��܂���");
	}
	Property::Property(Property* base, scope* sp)
	{
		this->type = new Type(_Property);
		if (base->getter)this->getter = new Function(base->getter, sp);
		else this->getter = nullptr;
		if (base->setter)this->setter = new Function(base->setter, sp);
		else this->setter = nullptr;
		this->getqualifier = base->getqualifier;
		this->setqualifier = base->setqualifier;
	}
}
