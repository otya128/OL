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
	bool Class::trygetMember(std::string &name, langObject& obj, lang::scope *access)
	{
		return trygetMember(name.c_str(), obj, access);
	}
	bool Class::trygetMember(const char* name, langObject& obj, lang::scope *access)
	{
		if (this->thisscope->variable.definedVar(name, access))
		{
			obj = this->thisscope->variable(name, access);
			return true;
		}
		else
		{
			if (this->base)
				return this->base->trygetMember(name, obj, access);
			return false;
		}
	}
	langObject Class::getMember(std::string& name, lang::scope *access)
	{
		return getMember(name.c_str(), access);
	}
	langObject Class::getMember(const char* name, lang::scope *access)
	{
		if (this->thisscope->variable.definedVar(name, access))
		{
			return this->thisscope->variable(name, access);
		}
		else
		{
			if (this->base)return this->base->getMember(name, access);
			throw_langRuntimeException("%sというmemberは存在しません", name);
		}
	}
	langObject Class::setMember(std::string& name, langObject obj, lang::scope *access)
	{
		return setMember(name.c_str(), obj, access);
	}
	langObject Class::setMember(const char* name, langObject obj, lang::scope *access)
	{
		if (this->thisscope->variable.definedVar(name, access))
		{
			return this->thisscope->variable.set(name, obj, access);
		}
		else
		{
			if (base)
				return this->base->setMember(name, obj, access);
			throw_langRuntimeException("%sというmemberは存在しません %s",name,obj->toString().c_str());
		}
	}
	ClassObject::ClassObject(Class* type) : Class(type), staticClass(type)//type->name,type->index,type->member,type->scope)
	{
		if (type->base)
		{
			this->base = type->base->CreateObject(type->base);//new ClassObject(type->base);
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
				langObject func;
				if (((langClass)obj1)->trygetMember("bracket", func, ((langClass)obj1)->thisscope))
				{
					if (func is _Function)
					{
						return ((langFunction)func)->call(&obj2);
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
				langObject func;
				if (((langClass)obj1)->trygetMember("bracketequal", func, ((langClass)obj1)->thisscope))
				{
					if (func is _Function)
					{
						return ((langFunction)func)->call(&obj2);
					}
				}
				break;
		}
		throw langRuntimeException((std::string(obj1->type->name) + "[" + "]=" + "出来ない").c_str());
	}
	langObject ArrayBuffer_Ctor(NativeFunction* func,std::vector<langObject> &arg)
	{
		ArrayBufferClassObject* _this = (ArrayBufferClassObject*)func->thisscope->_this;
		if (arg.size() != 1)
		{
			throw langRuntimeException("引数の数が違う");
		}
		size_t size = Int::toInt(arg[0]);
		ArrayBufferSetSize(_this,size);
		ArrayBufferSetPointer(_this,new char[size]);
		memset(ArrayBufferGetPointer(_this), 0, size);
		return NULLOBJECT;
	}
	langObject ArrayBuffer_Bracket(NativeFunction* func, std::vector<langObject> &arg)
	{
		if (arg.size() != 2)
		{
			throw langRuntimeException("引数の数が違う");
		}
		ArrayBufferClassObject* _this = (ArrayBufferClassObject*)func->thisscope->_this;
		unsigned char *ptr = (unsigned char*)ArrayBufferGetPointer(_this);
		int i1 = Int::toInt(arg[0]);
		unsigned char *obj = &ptr[i1];
		langObject i2 = arg[1];
		if (i2->type->TypeEnum == _Type)
		{
			ObjectType* type = (ObjectType*)i2;
			switch (type->TypeClass.TypeEnum)
			{
				case lang::_Object:
					break;
				case lang::_Int:
					return newInt(*(int*)obj);
				case lang::_String:
					return newString((char*)*((size_t*)obj));
				case lang::_Char:
					return new Char(*(char*)obj);
				case lang::_WChar:
					return new WChar(*(wchar_t*)obj);
				case lang::_Double:
					return newDouble(*(double*)obj);
				case lang::_Array:
					break;
				case lang::_Class:
					break;
				case lang::_ClassObject:
					break;
				default:
					throw langRuntimeException("get arraybuffer 不可");
					break;
			}
		}
		return NULLOBJECT;
	}
	langObject ArrayBuffer_BracketSet(NativeFunction* func, std::vector<langObject> &arg)
	{
		if (arg.size() != 2)
		{
			throw langRuntimeException("引数の数が違う");
		}
		ArrayBufferClassObject* _this = (ArrayBufferClassObject*)func->thisscope->_this;
		unsigned char *ptr = (unsigned char*)ArrayBufferGetPointer(_this);
		int i1 = Int::toInt(arg[0]);
		unsigned char *obj = &ptr[i1];
		langObject i2 = arg[1];
		switch (i2->type->TypeEnum)
		{
			case lang::_Object:
				throw langRuntimeException("set arraybuffer 不可");
				break;
			case lang::_Int:
				*((int*)obj) = Int::toInt(i2);
				break;
			case lang::_String:
				;
				{
					langString str = (langString)i2;
					*((size_t*)obj) = (size_t)str->getPointer();
				}
				break;
			case lang::_Char:
				*((char*)obj) = Char::toChar(i2);
				break;
			case lang::_WChar:
				*((wchar_t*)obj) = WChar::toWChar(i2);
				break;
			case lang::_Double:
				*((char*)obj) = Double::toDouble(i2);
				break;
			case lang::_Array:
				break;
			case lang::_Class:
				break;
			case lang::_Function:
				break;
			case lang::_ClassObject:
				break;
			case lang::_BuiltFunc:
				break;
			case lang::_Type:
				break;
			case lang::_Property:
				break;
			default:
				break;
		}
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
		this->member->push_back(membertypeitem(std::string("bracketequal"), std::pair<langObject, qualifier>(new NativeFunction(ArrayBuffer_BracketSet), public_)));
	}
	ArrayBufferClassObject::ArrayBufferClassObject() : ClassObject(lang::ClassArrayBufferClass)
	{
		nonunique = false;
	}
	ArrayBufferClassObject::~ArrayBufferClassObject()
	{
		if (this->ptr != this && !nonunique) delete this->ptr;
	}
	langClassObject ArrayBufferClass::CreateObject(Class* type)
	{
		return new ArrayBufferClassObject();
	}
	Class *ClassArrayBufferClass;
	//void ArrayBufferClassObject::setPointer(void* ptr)
	//{
	//	langClassObject cc = this;
	//	while (cc)
	//	{
	//		if (cc->staticClass == lang::ClassArrayBufferClass)
	//		{
	//			((ArrayBufferClassObject*)cc)->ptr = ptr;
	//			return;
	//		}
	//		cc = (langClassObject)cc->base;
	//	}
	//	throw langRuntimeException("what");
	//}
	//void* ArrayBufferClassObject::getPointer()
	//{
	//	langClassObject cc = this;
	//	while (cc)
	//	{
	//		if (cc->staticClass == lang::ClassArrayBufferClass)
	//		{
	//			return ((ArrayBufferClassObject*)cc)->ptr;
	//		}
	//		cc = (langClassObject)cc->base;
	//	}
	//	throw langRuntimeException("what");
	//}
	size_t ArrayBufferGetSize(langClassObject this_)
	{
		langClassObject cc = this_;
		while (cc)
		{
			if (cc->staticClass == lang::ClassArrayBufferClass)
			{
				return ((ArrayBufferClassObject*)cc)->size;
			}
			cc = (langClassObject)cc->base;
		}
		throw langRuntimeException("what");
	}
	size_t ArrayBufferSetSize(langClassObject this_, size_t t)
	{
		langClassObject cc = this_;
		while (cc)
		{
			if (cc->staticClass == lang::ClassArrayBufferClass)
			{
				return ((ArrayBufferClassObject*)cc)->size = t;
			}
			cc = (langClassObject)cc->base;
		}
		throw langRuntimeException("what");
	}
	void* ArrayBufferGetPointer(langClassObject this_)
	{
		langClassObject cc = this_;
		while (cc)
		{
			if (cc->staticClass == lang::ClassArrayBufferClass)
			{
				return ((ArrayBufferClassObject*)cc)->getPointer();
			}
			cc = (langClassObject)cc->base;
		}
		throw langRuntimeException("what");
	}
	void ArrayBufferSetPointer(langClassObject this_, void* ptr)
	{
		langClassObject cc = this_;
		while (cc)
		{
			if (cc->staticClass == lang::ClassArrayBufferClass)
			{
				((ArrayBufferClassObject*)cc)->setPointer(ptr);
				return;
			}
			cc = (langClassObject)cc->base;
		}
		throw langRuntimeException("what");
	}
	bool ArrayBufferSetNonUnique(langClassObject this_, bool unique)
	{
		langClassObject cc = this_;
		while (cc)
		{
			if (cc->staticClass == lang::ClassArrayBufferClass)
			{
				return ((ArrayBufferClassObject*)cc)->nonunique = true;
			}
			cc = (langClassObject)cc->base;
		}
		throw langRuntimeException("what");
	}
}
