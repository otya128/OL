#include "stdafx.h"
#include "Class.h"
#include "scope.h"
#include "lang.h"
#include "Function.h"
#include "langException.h"
namespace lang
{
	Class::Class(std::string name, int index, membertype member, lang::scope* scope, membertype staticmember)
	{
		this->base = nullptr;
		this->scope = scope;
		this->scope->refinc();
		this->type = new Type(PreType::_Class, (char*)"class");
		this->name = name;
		this->index = index;
		this->member = member;
		this->finalize = nullptr;
#if CPP11
		for (auto i : *member)
		{
#else
		for( auto it = member->begin(); it != member->end(); ++it )
		{
			auto i = *it;
#endif
			if (i.first == "finalize")
			{
				if (i.second is _Function)
				{
					this->finalize = (langFunction)i.second;
					break;
				}
			}
		}

		thisscope = new lang::scope(this->scope->parsers, this->scope, nullptr);
		this->thisscope->refinc();
		FOREACH(i, *staticmember)//        foreach_(var_ i in_ *staticmember)//C# style foreach
			//        {
		if (i.second->type->TypeEnum == _Function)
		{
			auto buf = new Function((langFunction)i.second, this->thisscope);
			buf->scope = thisscope;
			this->thisscope->variable.add(i.first, buf);
		}
		else
			this->thisscope->variable.add(i.first, i.second);
		ENDFOREACH
			/*
			if(this->finalize != nullptr)
			{
			this->finalize = new Function((langFunction)this->finalize,this->thisscope);
			this->finalize->scope = thisscope;
			this->thisscope->variable.add("finalize", this->finalize);
			lang::gc->uncontroll(this->finalize);
			}*/
			delete staticmember;
	}
	Class::Class(Class* clas)
	{
		this->base = clas->base;
		this->type = new Type(PreType::_Class, (char*)"class");
		this->name = clas->name;
		this->index = clas->index;
		this->member = clas->member;
		this->scope = clas->scope;
		this->finalize = nullptr;
	}
	std::string Class::toString()
	{
		return "Class:" + this->name;
	}
	Class::~Class(void)
	{
		if (this->type->TypeEnum == PreType::_Class)
		{
			delete this->member;
			this->scope->refdec();
		}//delete this->name;
#if _DEBUG
		if (lang::gc_view)std::cout << "remove" << this << this->type->name << std::endl;
#endif
		this->thisscope->refdec();
		//delete this->type->name;
	}
	langObject Class::getMember(std::string& name)
	{
		if (this->thisscope->variable.definedVar(name))
		{
			return this->thisscope->variable[name];
		}
		else
		{
			if (this->base)return this->base->getMember(name);
			return NULLOBJECT;
		}
	}
	langObject Class::setMember(std::string& name, langObject obj)
	{
		if (this->thisscope->variable.definedVar(name))
		{
			return this->thisscope->variable.set(name, obj);
		}
		else
		{
			return this->base->setMember(name, obj);
		}
	}
	ClassObject::ClassObject(Class* type) : Class(type), staticClass(type)//type->name,type->index,type->member,type->scope)
	{
		if (type->base)
		{
			this->base = new ClassObject(type->base);
		}
		this->scope = type->scope;
		thisscope = new lang::scope(this->scope->parsers, this->scope, this);
		this->thisscope->refinc();
		this->type->TypeEnum = _ClassObject;
		FOREACH(i, *member)//foreach_(var_ i in_ *this->member)//C# style foreach
			//{
		if (i.second->type->TypeEnum == _Function)
		{
			auto buf = new Function((langFunction)i.second, this->thisscope);
			buf->scope = thisscope;
			this->thisscope->variable.add(i.first, buf);
		}
		else
			this->thisscope->variable.add(i.first, i.second);
		ENDFOREACH//}
		if (type->finalize != nullptr)
		{
			this->finalize = new Function((langFunction)type->finalize, this->thisscope);
			this->finalize->scope = thisscope;
			this->thisscope->variable.add("finalize", this->finalize);
			lang::gc->uncontroll(this->finalize);
		}

	}

	std::string ClassObject::toString(void)
	{
		if (this->thisscope->variable["ToString"] is _Function)
		{
			auto arg = std::vector<langObject>();//普通にローカル変数のポインタ私で良かった
			auto result = ((langFunction)this->thisscope->variable["ToString"])->call(&arg);
			return result->toString();
			/*
			auto arg = new std::vector<langObject>;//例外処理で逆に冗長になる例
			try
			{
			auto result = ((langFunction)this->thisscope->variable["ToString"])->call(arg);
			delete arg;
			return result->toString();
			}
			catch(...)
			{
			delete arg;
			throw;
			}*/
		}
		return "ClassObject:" + this->name;//Class::toString();
	}
	ClassObject::~ClassObject(void)
	{
		if (this->finalize)//もう終了してたらファイナライザは呼ばれない
		{
			if (lang::running)
			{
				std::vector<langObject> arg;
				this->finalize->call(&arg);
			}
			delete this->finalize;
		}
		//this->scope->refdec();//scopeじゃなくてthisscopeでは
	}

	langObject Object::bracket(langObject obj1, std::vector<langObject> obj2)
	{
		switch (obj1->type->TypeEnum)
		{
			case PreType::_ClassObject:
				if (((langClass)obj1)->thisscope->variable.definedVar("bracket"))
				{
					auto func = (langFunction)((langClass)obj1)->thisscope->variable["bracket"];
					if (func is _Function)
					{
						return func->call(&obj2);
					}
				}
				break;
		}
		throw langRuntimeException((std::string(obj1->type->name) + "[" + "]出来ない").c_str());
	}
	langObject Object::bracketequal(langObject obj1, std::vector<langObject> obj2)
	{
		switch (obj1->type->TypeEnum)
		{
			case PreType::_ClassObject:
				if (((langClass)obj1)->thisscope->variable.definedVar("bracketequal"))
				{
					auto func = (langFunction)((langClass)obj1)->thisscope->variable["bracketequal"];
					if (func is _Function)
					{
						return func->call(&obj2);
					}
				}
				break;
		}
		throw langRuntimeException((std::string(obj1->type->name) + "[" + "]=" + "出来ない").c_str());
	}
}
