#include "stdafx.h"
#include "Class.h"
#include "scope.h"
#include "lang.h"
#include "Function.h"
#include "langException.h"
namespace lang
{
	class scope;
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
				if (i.second.first is _Function)
				{
					this->finalize = (langFunction)i.second.first;
					break;
				}
			}
		}

		thisscope = new lang::scope(this->scope->parsers, this->scope, nullptr);
		this->thisscope->refinc();
		FOREACH(i, *staticmember)//        foreach_(var_ i in_ *staticmember)//C# style foreach
			//        {
		if (i.second.first->type->TypeEnum == _Function)
		{
			auto buf = Function::CopyFunction((langFunction)i.second.first, this->thisscope);
			buf->scope = thisscope;
			this->thisscope->variable.add(i.first, buf, i.second.second);
		}
		else
		if (i.second.first->type->TypeEnum == _Property)
		{
			auto buf = new Property((Property*)i.second.first, thisscope);
			this->thisscope->variable.add(i.first, buf, i.second.second);
		}
		else
			this->thisscope->variable.add(i.first, i.second.first, i.second.second);
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
	bool Class::trygetMember(std::string& name, langObject& obj, lang::scope *access)
	{
		if (this->thisscope->variable.definedVar(name, access))
		{
			obj = this->thisscope->variable(name, access);
			return true;
		}
		else
		{
			if (this->base)return this->base->trygetMember(name, obj, access);
			return false;
		}
	}
	langObject Class::getMember(std::string& name, lang::scope *access)
	{
		if (this->thisscope->variable.definedVar(name, access))
		{
			return this->thisscope->variable(name, access);
		}
		else
		{
			if (this->base)return this->base->getMember(name, access);
			throw_langRuntimeException("%sというmemberは存在しません", name.c_str());
		}
	}
	langObject Class::setMember(std::string& name, langObject obj, lang::scope *access)
	{
		if (this->thisscope->variable.definedVar(name, access))
		{
			return this->thisscope->variable.set(name, obj, access);
		}
		else
		{
			if (base)
			return this->base->setMember(name, obj, access);
			throw_langRuntimeException("%sというmemberは存在しません %s",name.c_str(),obj->toString().c_str());
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
		if (i.second.first->type->TypeEnum == _Function)
		{
			if (i.second.first != type->finalize)
			{
				auto buf = Function::CopyFunction((langFunction)i.second.first, this->thisscope);
				buf->scope = thisscope;
				this->thisscope->variable.add(i.first, buf, i.second.second);
			}
		}
		else
		if (i.second.first->type->TypeEnum == _Property)
		{
			auto buf = new Property((Property*)i.second.first, thisscope);
			this->thisscope->variable.add(i.first, buf, i.second.second);
		}
		else
			this->thisscope->variable.add(i.first, i.second.first, i.second.second);
		ENDFOREACH//}
		if (type->finalize != nullptr)
		{
			this->finalize = Function::CopyFunction((langFunction)type->finalize, this->thisscope);
			this->finalize->scope = thisscope;
			this->thisscope->variable.add("finalize", this->finalize);
			lang::gc->uncontroll(this->finalize);
		}

	}
	langClassObject Class::CreateObject(Class* type)
	{
		return new ClassObject(type);
	}
	std::string ClassObject::toString(void)
	{
		langObject tostr;
		std::string tostrs("ToString");
		if (this->trygetMember(tostrs,tostr,this->thisscope) && tostr is _Function)//this->thisscope->variable["ToString"] is _Function)
		{
			auto arg = std::vector<langObject>();//普通にローカル変数のポインタ私で良かった
			auto result = ((langFunction)/*this->thisscope->variable["ToString"]*/tostr)->call(&arg);
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
				if (((langClass)obj1)->thisscope->variable.definedVar("bracket", ((langClass)obj1)->thisscope))
				{
					auto func = (langFunction)((langClass)obj1)->thisscope->variable("bracket", ((langClass)obj1)->thisscope);
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
				if (((langClass)obj1)->thisscope->variable.definedVar("bracketequal", ((langClass)obj1)->thisscope))
				{
					auto func = (langFunction)((langClass)obj1)->thisscope->variable("bracketequal", ((langClass)obj1)->thisscope);
					if (func is _Function)
					{
						return func->call(&obj2);
					}
				}
				break;
		}
		throw langRuntimeException((std::string(obj1->type->name) + "[" + "]=" + "出来ない").c_str());
	}
	langObject ArrayBuffer_Ctor(NativeFunction* func,std::vector<langObject> &arg)
	{
		std::cout << "DEBUg";
		langClassObject _this = func->thisscope->_this;
		if (arg.size() != 1)
		{
			throw langRuntimeException("引数の数が違う");
		}
		int size = Int::toInt(arg[0]);
		_this->setPointer(new char[size]);
		memset(_this->getPointer(), 0, size);
		return NULLOBJECT;
	}
	langObject ArrayBuffer_Bracket(NativeFunction* func, std::vector<langObject> &arg)
	{
		std::cout << "DEBUg";
		if (arg.size() != 2)
		{
			throw langRuntimeException("引数の数が違う");
		}
		langClassObject _this = func->thisscope->_this;
		return NULLOBJECT;
	}
	langObject ArrayBuffer_BracketSet(NativeFunction* func, std::vector<langObject> &arg)
	{
		std::cout << "DEBUg";
		langClassObject _this = func->thisscope->_this;
		return NULLOBJECT;
	}
	ArrayBufferClass::ArrayBufferClass(lang::scope *scopec)
	{
		this->finalize = nullptr;
		this->base = nullptr;
		this->scope = scopec;
		this->type = new Type(_Class);
		this->member = new membertype_;
		this->member->push_back(membertypeitem(std::string("ctor"), std::pair<langObject, qualifier>(new NativeFunction(ArrayBuffer_Ctor), public_)));
		this->member->push_back(membertypeitem(std::string("bracket"), std::pair<langObject, qualifier>(new NativeFunction(ArrayBuffer_Bracket), public_)));
		this->member->push_back(membertypeitem(std::string("bracketset"), std::pair<langObject, qualifier>(new NativeFunction(ArrayBuffer_BracketSet), public_)));
	}
	ArrayBufferClassObject::ArrayBufferClassObject() : ClassObject(lang::ClassArrayBufferClass)
	{
	}
	ArrayBufferClassObject::~ArrayBufferClassObject()
	{
		if (this->ptr != this) delete this->ptr;
	}
	langClassObject ArrayBufferClass::CreateObject(Class* type)
	{
		return new ArrayBufferClassObject();
	}
	Class *ClassArrayBufferClass;
}
