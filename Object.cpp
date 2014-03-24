#pragma once
#include "stdafx.h"
#include "Object.h"
//#include "parserEnum.h"
#include <sstream>
#include <memory>
#include "GC.h"
#include "Function.h"
#include "Class.h"
#include "langException.h"
#include "Array.h"
#include <stdlib.h>
#define newInt(a) new Int(a)
#define newString(a) new String(a)
#ifndef _WIN32
#define wcstombs_s(a,b,c,d,e) wcstombs(b,d,e)
#endif
/*#define langObject langObject
#define langInt std::shared_ptr<Int>
#define langString std::shared_ptr<String>*/
namespace lang
{
	gabekore* gc;
	Object::Object(void* ptr)
	{
		if (lang::gc != nullptr)
		{
			gc->addObject(this);
		}
		this->type = new Type(PreType::_Object);
		this->ptr = ptr;
	}
	Object::Object(void)
	{
		this->ptr = this;
#if _DEBUG
		if (gc_view) std::cout << "add" << this << std::endl;//<<std::endl;
#endif
		if (lang::gc != nullptr)
		{
			gc->addObject(this);
		}
		this->type = (Type*)ObjectTypeClass;//new Type(PreType::_Object);
	}

	std::string _toString(Object* arg)
	{
		// return arg->toString();for(i=0;i<10;i++){print(i);}
		if (arg == nullptr)return "null";
		/*if(arg->type->TypeEnum==PreType::_Int)
		{
		std::stringstream ss;
		ss<<(static_cast<Int*>(arg))->getInt();
		return (ss.str());
		}
		if(arg->type->TypeEnum==PreType::_String)
		{
		return *(static_cast<String*>(arg))->getString();
		}
		if(arg->type->TypeEnum==PreType::_Function)
		{
		return *(static_cast<Function*>(arg))->name;
		}*/
		return "";
	}
#define GABEKORE 1
	Object::~Object(void)
	{
#if GABEKORE
#if _DEBUG
		if (gc_view) std::cout << "がべこれ中..." << this << "が滅亡しました...ptr" << this->ptr << "\t" << this->type->name << "\t" << _toString(this) << std::endl;//<<std::endl;
#endif
#endif
		if (type != (Type*)ObjectTypeClass)delete this->type;
		//delete this->ptr;
	}
	SpecialFunction* object_tostr = new SpecialFunction(0);
	SpecialFunction* string_substr = new SpecialFunction(1);
	langObject Object::getMember(std::string& name)
	{
		return object_tostr;
	}
	langObject Object::setMember(std::string& name, langObject obj)
	{
		//設定できるメンバなんてない
		return nullptr;
	}
	void* Object::getPointer(void)
	{
		return this->ptr;
	}
	void Object::setPointer(void* ptr)
	{
		this->ptr = ptr;
		return;
	}
	std::string Object::toString()
	{
		return this->type->name;
	}
	Int::Int(int i)
	{
		this->type = new Type(PreType::_Int);
		this->ptr = (void*)new int(i);
#if _DEBUG
		debuggg = i;
#endif
	}
	Int::~Int(void)
	{
		//delete this->type;
		delete (int*)this->ptr;
	}
	int Int::getInt(void)
	{
		return *(int*)(this->ptr);
	}
	void Int::setInt(int i)
	{
		delete this->ptr;
		this->ptr = new int(i);
#if _DEBUG
		debuggg = i;
#endif
		return;
	}
	std::string Int::toString()
	{
		std::stringstream ss;
		ss << this->getInt();
		return (ss.str());
	}

	String::String(std::string* i)
	{
		this->type = new Type(PreType::_String);
		this->ptr = (void*)new std::string(*i);

	}
	String::String(std::string &i)
	{
		this->type = new Type(PreType::_String);
		this->ptr = (void*)new std::string(i);
	}
	String::String(std::string i)
	{
		this->type = new Type(PreType::_String);
		this->ptr = (void*)new std::string(i);
	}
	String::String(char* i)
	{
		this->type = new Type(PreType::_String);
		this->ptr = (void*)new std::string(i);
	}
	String::~String(void)
	{
		//delete this->type;
#if _DEBUG
		if (lang::gc_view)
			std::cout << *(std::string*)this->ptr;
#endif
		delete (std::string*)this->ptr;
	}
	std::string* String::getString(void)
	{
		return (std::string*)(this->ptr);
	}
	void String::setString(std::string* i)
	{
		delete this->ptr;
		this->ptr = (void*)i;
		return;
	}
	std::string String::toString(void)
	{
		return *((std::string*)(this->ptr));
	}
	langObject String::getMember(std::string& name)
	{
		if (name == "ToString")
			return object_tostr;
		else if (name == "Substring")
			return string_substr;
	}

	int Int::toInt(langObject obj)
	{
		if (obj->type->TypeEnum == PreType::_Int)
			return (static_cast<Int*>(obj))->getInt();
		if (obj->type->TypeEnum == PreType::_Double)
			return (static_cast<Double*>(obj))->getDouble();
		return 0;//変換不可
	}
	char Char::toChar(langObject obj)
	{
		if (obj->type->TypeEnum == PreType::_Char)
			return (static_cast<Char*>(obj))->getChar();
		return 0;//変換不可
	}
	Char::Char(char i)
	{
		this->type = new Type(PreType::_Char);
		this->chr = i;
		this->ptr = &this->chr;//(void*)new char(i);
	}
	Char::~Char(void)
	{
	}
	char Char::getChar(void)
	{
		return *(char*)(this->ptr);
	}
	void Char::setChar(char i)
	{
		this->chr = i;
		this->ptr = &this->chr;
		return;
	}
	std::string Char::toString()
	{
		std::stringstream ss;
		ss << this->getChar();
		return (ss.str());
	}

	wchar WChar::toWChar(langObject obj)
	{
		if (obj->type->TypeEnum == PreType::_Char)
			return (static_cast<Char*>(obj))->getChar();
		return 0;//変換不可
	}
	WChar::WChar(wchar i)
	{
		this->type = new Type(PreType::_WChar);
		this->chr = i;
		this->ptr = &this->chr;//(void*)new char(i);
	}
	WChar::~WChar(void)
	{
	}
	wchar WChar::getWChar(void)
	{
		return *(char*)(this->ptr);
	}
	void WChar::setWChar(wchar i)
	{
		this->chr = i;
		this->ptr = &this->chr;
		return;
	}
	std::string WChar::toString()
	{
		__v('ω')
			//変換文字列格納バッファ
			/*char wStrC[3];
			size_t wLen = 0;
			errno_t err = 0;
			//ロケール指定
			setlocale(LC_ALL,"japanese");
			//変換
			err = wcstombs_s(&wLen, wStrC, 2, &this->chr, _TRUNCATE);*/
			char mbs[3];//char *mbs = new char[src.length() * MB_CUR_MAX + 1];
		wcstombs_s(NULL, mbs, 3, &this->chr, 2);
		/*dest = mbs;
		delete [] mbs;*/
		std::stringstream ss;
		ss << mbs;
		return (ss.str());
	}
	double Double::toDouble(langObject obj)
	{
		if (obj->type->TypeEnum == PreType::_Double)
			return (static_cast<Double*>(obj))->getDouble();
		if (obj->type->TypeEnum == PreType::_Int)
			return (static_cast<Int*>(obj))->getInt();
		return 0;//変換不可
	}
	Double::Double(double i)
	{
		this->type = new Type(PreType::_Double);
		this->dbl = i;
		this->ptr = &this->dbl;//(void*)new char(i);
	}
	Double::~Double(void)
	{
	}
	double Double::getDouble(void)
	{
		return this->dbl;
	}
	void Double::setDouble(double i)
	{
		this->dbl = i;
		this->ptr = &this->dbl;
		return;
	}
	std::string Double::toString()
	{
		std::stringstream ss;
		ss << this->getDouble();
		return (ss.str());
	}

#define LANG_OPERA_DEBUG_CHECK 
#define LANG_OPERA_DEBUG_SINGLE_CHECK 
#if _DEBUG
#define LANG_OPERA_DEBUG_CHECK if (!obj1 || !obj2)throw langRuntimeException(__FUNCTION__"式がありません。");
#define LANG_OPERA_DEBUG_SINGLE_CHECK if (!obj1)throw langRuntimeException(__FUNCTION__"式がありません。");
#endif
#define OPERA2ARG(name) {auto clas = (langClassObject)obj1;\
	auto func = (langFunction)clas->thisscope->variable[name]; \
	if (func != nullptr && func is _Function)\
	{\
	auto vec = new std::vector<langObject>(); \
	vec->push_back(obj2); \
	try\
	{\
	auto ret = func->call(vec); \
	delete vec; \
	return ret; \
	}\
	catch (...)\
	{\
	delete vec; \
	throw; \
	}\
	}\
			else\
			throw langRuntimeException((std::string("関数") + name + "が定義されていません").c_str()); }
#define OPERA2ARGSINGLE(name) {auto clas = (langClassObject)obj1;\
	auto func = (langFunction)clas->thisscope->variable[name]; \
	if (func != nullptr && func is _Function)\
	{auto arg = std::vector<langObject>(); \
	auto ret = func->call(&arg); \
	return ret; \
	}\
			else\
			throw langRuntimeException((std::string("関数") + name + "が定義されていません").c_str()); }

#define OPERA2ARG2(name) {auto clas = (langClassObject)obj2;\
	auto func = (langFunction)clas->thisscope->variable[name]; \
	if (func != nullptr && func is _Function)\
	{\
	auto vec = new std::vector<langObject>(); \
	vec->push_back(obj1); \
	try\
	{\
	auto ret = func->call(vec); \
	delete vec; \
	return ret; \
	}\
	catch (...)\
	{\
	delete vec; \
	throw; \
	}\
	}\
			else\
			throw langRuntimeException((std::string("関数") + name + "が定義されていません").c_str()); }
	/*
	{auto clas = (langClassObject)obj1;
	auto func = (langFunction)clas->thisscope->variable[name];
	if(func != nullptr && func is _Function)
	{
	auto vec = new std::vector<langObject>();
	vec->push_back(obj2);
	try
	{
	auto ret = func->call(vec);
	delete vec;
	return ret;
	}
	catch(...)
	{
	delete vec;
	throw;
	}
	}
	else
	throw langRuntimeException((std::string("関数")+name+"が定義されていません").c_str());}
	*/
	langObject Object::bracket(langObject obj1, langObject obj2)
	{
		LANG_OPERA_DEBUG_CHECK
			switch (obj1->type->TypeEnum)
		{
				case PreType::_Array:
					return ((langArray)obj1)->ary[Int::toInt(obj2)];
					break;
		}
		throw langRuntimeException((std::string(obj1->type->name) + "[" + obj2->type->name + "]出来ない").c_str());
		//変換不可
	}
	langObject Object::bracketequal(langObject obj1, langObject obj2, langObject obj3)
	{
		LANG_OPERA_DEBUG_CHECK
			switch (obj1->type->TypeEnum)
		{
				case PreType::_Array:
					return ((langArray)obj1)->ary[Int::toInt(obj2)] = obj3;
					break;
		}
		throw langRuntimeException((std::string(obj1->type->name) + "[" + obj2->type->name + "]出来ない").c_str());
		//変換不可
	}
	langObject Object::plus(langObject obj1, langObject obj2)
	{
		LANG_OPERA_DEBUG_CHECK
			switch (obj1->type->TypeEnum)
		{
				case PreType::_Int:
					return newInt(Int::toInt(obj1) + Int::toInt(obj2));
				case PreType::_Double:
					return newDouble(Double::toDouble(obj1) + Double::toDouble(obj2));
				case _String:
					return newString((obj1->toString() + obj2->toString()));
				case PreType::_ClassObject:
				{auto clas = (langClassObject)obj1;
				auto func = (langFunction)clas->thisscope->variable["plus"];
				if (func != nullptr && func is _Function)
				{
					auto vec = new std::vector<langObject>();
					vec->push_back(obj2);
					try
					{
						auto ret = func->call(vec);
						delete vec;
						return ret;
					}
					catch (...)
					{
						delete vec;
						throw;
					}
				}
				else
					throw langRuntimeException((std::string("関数plus") + "が定義されていません").c_str()); }//OPERA2ARG("plus")
					//break;
		}
		throw langRuntimeException((std::string(obj1->type->name) + "+" + obj2->type->name + "出来ない").c_str());
		//変換不可
	}
	langObject Object::minus(langObject obj1, langObject obj2)
	{
		LANG_OPERA_DEBUG_CHECK
			switch (obj1->type->TypeEnum)
		{
				case PreType::_Int:
					return newInt(Int::toInt(obj1) - Int::toInt(obj2));
				case PreType::_Double:
					return newDouble(Double::toDouble(obj1) - Double::toDouble(obj2));
				case PreType::_ClassObject:
					OPERA2ARG("minus")
		}
		throw langRuntimeException((std::string(obj1->type->name) + "-" + obj2->type->name + "出来ない").c_str());
		//変換不可
	}
	langObject Object::unaryminus(langObject obj1)
	{
		LANG_OPERA_DEBUG_SINGLE_CHECK
			switch (obj1->type->TypeEnum)
		{
				case PreType::_Int:
					return newInt(-Int::toInt(obj1));
				case PreType::_Double:
					return newDouble(-Double::toDouble(obj1));
				case PreType::_ClassObject:
					OPERA2ARGSINGLE("unaryminus")
		}
		throw langRuntimeException(("-" + std::string(obj1->type->name) + "出来ない").c_str());
		//変換不可
	}
	langObject Object::unaryplus(langObject obj1)
	{
		LANG_OPERA_DEBUG_SINGLE_CHECK
			switch (obj1->type->TypeEnum)
		{
				case PreType::_Int:
					return newInt(+Int::toInt(obj1));
				case PreType::_Double:
					return newDouble(+Double::toDouble(obj1));
				case PreType::_ClassObject:
					OPERA2ARGSINGLE("unaryplus")
		}
		throw langRuntimeException(("+" + std::string(obj1->type->name) + "出来ない").c_str());
		//変換不可
	}
	langObject Object::multiply(langObject obj1, langObject obj2)
	{
		LANG_OPERA_DEBUG_CHECK
			switch (obj1->type->TypeEnum)
		{
				case PreType::_Int:
					return newInt(Int::toInt(obj1) * Int::toInt(obj2));
				case PreType::_Double:
					return newDouble(Double::toDouble(obj1) * Double::toDouble(obj2));
				case PreType::_ClassObject:
					OPERA2ARG("multiply")
		}
		throw langRuntimeException((std::string(obj1->type->name) + "*" + obj2->type->name + "出来ない").c_str());
		//変換不可
	}
	langObject Object::division(langObject obj1, langObject obj2)
	{
		LANG_OPERA_DEBUG_CHECK
			switch (obj1->type->TypeEnum)
		{
				case PreType::_Int:
					return newInt(Int::toInt(obj1) / Int::toInt(obj2));
				case PreType::_Double:
					return newDouble(Double::toDouble(obj1) / Double::toDouble(obj2));
				case PreType::_ClassObject:
					OPERA2ARG("multiply")
		}
		throw langRuntimeException((std::string(obj1->type->name) + "/" + obj2->type->name + "出来ない").c_str());
		//変換不可
	}
	langObject Object::greater(langObject obj1, langObject obj2)
	{
		LANG_OPERA_DEBUG_CHECK
			switch (obj1->type->TypeEnum)
		{
				case PreType::_Int:
					if (obj2 is _Int)return newInt(Int::toInt(obj1) > Int::toInt(obj2));
					else return newInt(obj1->getPointer() > obj2->getPointer());
				case PreType::_Double:
					if (obj2 is _Double)return newInt(Double::toDouble(obj1) > Double::toDouble(obj2));
					else return newInt(obj1->getPointer() > obj2->getPointer());
				case PreType::_ClassObject:
					OPERA2ARG("greater")
		}
		throw langRuntimeException((std::string(obj1->type->name) + ">" + obj2->type->name + "出来ない").c_str());
		//変換不可
	}
	langObject Object::less(langObject obj1, langObject obj2)
	{
		LANG_OPERA_DEBUG_CHECK
			switch (obj1->type->TypeEnum)
		{
				case PreType::_Int:
					if (obj2 is _Int)return newInt(Int::toInt(obj1) < Int::toInt(obj2));
					else return newInt(obj1->getPointer() < obj2->getPointer());
				case PreType::_Double:
					if (obj2 is _Double)return newInt(Double::toDouble(obj1) < Double::toDouble(obj2));
					else return newInt(obj1->getPointer() < obj2->getPointer());
				case PreType::_ClassObject:
					OPERA2ARG("less")
		}
		throw langRuntimeException((std::string(obj1->type->name) + "<" + obj2->type->name + "出来ない").c_str());
		//変換不可
	}
	langObject Object::greaterEqual(langObject obj1, langObject obj2)
	{
		LANG_OPERA_DEBUG_CHECK
			switch (obj1->type->TypeEnum)
		{
				case PreType::_Int:
					if (obj2 is _Int)return newInt(Int::toInt(obj1) >= Int::toInt(obj2));
					else return newInt(obj1->getPointer() >= obj2->getPointer());
				case PreType::_Double:
					if (obj2 is _Double)return newInt(Double::toDouble(obj1) >= Double::toDouble(obj2));
					else return newInt(obj1->getPointer() >= obj2->getPointer());
				case PreType::_ClassObject:
					OPERA2ARG("greaterEqual")
		}
		throw langRuntimeException((std::string(obj1->type->name) + ">=" + obj2->type->name + "出来ない").c_str());
	}
	langObject Object::lessEqual(langObject obj1, langObject obj2)
	{
		LANG_OPERA_DEBUG_CHECK
			switch (obj1->type->TypeEnum)
		{
				case PreType::_Int:
					if (obj2 is _Int)return newInt(Int::toInt(obj1) <= Int::toInt(obj2));
					else return newInt(obj1->getPointer() <= obj2->getPointer());
				case PreType::_Double:
					if (obj2 is _Double)return newInt(Double::toDouble(obj1) <= Double::toDouble(obj2));
					else return newInt(obj1->getPointer() <= obj2->getPointer());
				case PreType::_ClassObject:
					OPERA2ARG("lessEqual")
		}
		throw langRuntimeException((std::string(obj1->type->name) + "[" + obj1->toString() + "]<=[" + obj2->toString() + "]" + obj2->type->name + "出来ない").c_str());
	}
	langObject Object::equal(langObject obj1, langObject obj2)
	{
		LANG_OPERA_DEBUG_CHECK
			switch (obj1->type->TypeEnum)
		{
				case PreType::_Int:
					if (obj2 is _Int)return newInt(Int::toInt(obj1) == Int::toInt(obj2));
					else return newInt(obj1->getPointer() == obj2->getPointer());
				case PreType::_Double:
					if (obj2 is _Double)return newInt(Double::toDouble(obj1) == Double::toDouble(obj2));
					else return newInt(obj1->getPointer() == obj2->getPointer());
				case PreType::_ClassObject:
				{
											  auto clas = (langClassObject)obj1;
											  auto func = (langFunction)clas->thisscope->variable["equal"];
											  if (func != nullptr && func is _Function)
											  {
												  auto vec = new std::vector<langObject>();
												  vec->push_back(obj2);
												  try
												  {
													  auto ret = func->call(vec);
													  delete vec;
													  return ret;
												  }
												  catch (...)
												  {
													  delete vec;
													  throw;
												  }
											  }
											  else
												  return newInt(obj1 == obj2);
				}
				default:
					return newInt(obj1 == obj2);
					break;
		}
	}
	langObject Object::modulo(langObject obj1, langObject obj2)
	{
		LANG_OPERA_DEBUG_CHECK
			switch (obj1->type->TypeEnum)
		{
				case PreType::_Int:
					return newInt(Int::toInt(obj1) % Int::toInt(obj2));
				case PreType::_ClassObject:
					OPERA2ARG("modulo")
		}
		throw langRuntimeException((std::string(obj1->type->name) + "%" + obj2->type->name + "出来ない").c_str());
	}
	langObject Object::leftShift(langObject obj1, langObject obj2)
	{
		LANG_OPERA_DEBUG_CHECK
			switch (obj1->type->TypeEnum)
		{
				case PreType::_Int:
					if (obj2 is _ClassObject)
					{
						auto clas = (langClassObject)obj2;
						auto func = (langFunction)clas->thisscope->variable["leftShift"];
						if (func != nullptr && func is _Function)
						{
							auto vec = new std::vector<langObject>();
							vec->push_back(obj1);
							try
							{
								auto ret = func->call(vec);
								delete vec;
								return ret;
							}
							catch (...)
							{
								delete vec;
								throw;
							}
						}
						else
							throw langRuntimeException((std::string("関数leftShift") + "が定義されていません").c_str());
					}
					return newInt(Int::toInt(obj1) << Int::toInt(obj2));
				case PreType::_ClassObject:
					OPERA2ARG("leftShift")
				default:

					if (obj2 is _ClassObject)
					{
						auto clas = (langClassObject)obj2;
						auto func = (langFunction)clas->thisscope->variable["leftShift"];
						if (func != nullptr && func is _Function)
						{
							auto vec = new std::vector<langObject>();
							vec->push_back(obj1);
							try
							{
								auto ret = func->call(vec);
								delete vec;
								return ret;
							}
							catch (...)
							{
								delete vec;
								throw;
							}
						}
						else
							throw langRuntimeException((std::string("関数leftShift") + "が定義されていません").c_str());
					}
		}
		throw langRuntimeException((std::string(obj1->type->name) + "[" + obj1->toString() + "]<<[" + obj2->toString() + "]" + obj2->type->name + "出来ない").c_str());
	}
	langObject Object::rightShift(langObject obj1, langObject obj2)
	{
		LANG_OPERA_DEBUG_CHECK
			switch (obj1->type->TypeEnum)
		{
				case PreType::_Int:
					return newInt(Int::toInt(obj1) >> Int::toInt(obj2));
				case PreType::_ClassObject:
					OPERA2ARG("rightShift")
		}
		throw langRuntimeException((std::string(obj1->type->name) + ">>" + obj2->type->name + "出来ない").c_str());
	}

	langObject Object::inc(langObject obj1)
	{
		switch (obj1->type->TypeEnum)
		{
			case PreType::_Int:
				return newInt(Int::toInt(obj1) + 1);
			case PreType::_Double:
				return newDouble(Double::toDouble(obj1) + 1.0);
				//case PreType::_ClassObject:
				//    OPERA2ARG("inc")
		}
		throw langRuntimeException((std::string(obj1->type->name) + "++出来ない").c_str());
		//変換不可
	}
#pragma region TypeDefine
	ObjectType::ObjectType() : TypeClass(_Object)
	{
		this->type = new Type(PreType::_Type, "object");
	}
	langObject ObjectType::create(std::vector<langObject> &arg)
	{
		return new Object();
	}
	IntType::IntType() : TypeClass(_Int)
	{
		this->type = new Type(PreType::_Type, "int");
	}
	langObject IntType::create(std::vector<langObject> &arg)
	{
		return new Int(Int::toInt(arg[0]));
	}
	StringType::StringType() : TypeClass(_String)
	{
		this->type = new Type(PreType::_Type, "string");
	}
	langObject StringType::create(std::vector<langObject> &arg)
	{
		return new String(arg[0]->toString());
	}
	DoubleType::DoubleType() : TypeClass(_Double)
	{
		this->type = new Type(PreType::_Type, "double");
	}
	langObject DoubleType::create(std::vector<langObject> &arg)
	{
		return new Double(Double::toDouble(arg[0]));
	}
	CharType::CharType() : TypeClass(_Char)
	{
		this->type = new Type(PreType::_Type, "char");
	}
	langObject CharType::create(std::vector<langObject> &arg)
	{
		return new Char(Char::toChar(arg[0]));
	}
	WCharType::WCharType() : TypeClass(_WChar)
	{
		this->type = new Type(PreType::_Type, "wchar");
	}
	langObject WCharType::create(std::vector<langObject> &arg)
	{
		return new WChar(WChar::toWChar(arg[0]));
	}
	ArrayType::ArrayType() : TypeClass(_WChar)
	{
		this->type = new Type(PreType::_Type, "array");
	}
	langObject ArrayType::create(std::vector<langObject> &arg)
	{
		return new Array(Int::toInt(arg[0]));
	}
#pragma endregion
	langObject Object::_is(langObject obj1, langObject obj2)
	{
		if (obj2->type->TypeEnum == _Type || obj2->type->TypeEnum == _Class)
		switch (obj2->type->TypeEnum)
		{
			case _Type:
				if ((((ObjectType*)obj2)->TypeClass.TypeEnum == obj1->type->TypeEnum))return TRUEOBJECT;
				return FALSEOBJECT;
			case _Class:
				auto instance = (langClassObject)obj1;
				while (instance)
				{
					if (instance->staticClass == obj2)return TRUEOBJECT;
					instance = (langClassObject)instance->base;
				}
				break;
		}
		return FALSEOBJECT;
	}
	langObject Object::as(langObject obj1, langObject obj2)
	{
		if (obj2->type->TypeEnum == _Type || obj2->type->TypeEnum == _Class)
			switch (obj2->type->TypeEnum)
		{
				case _Type:
				{
							  auto objtype = (ObjectType*)obj2;
							  switch (objtype->TypeClass.TypeEnum)
							  {
								  //toint
								  case _Int:
									  switch (obj1->type->TypeEnum)
									  {
										  case _Int:
											  return newInt((int)((langInt)obj1)->getInt());
										  case _Double:
											  return newInt((int)((langDouble)obj1)->getDouble());
									  }
									  break;
									  //todouble
								  case _Double:
									  switch (obj1->type->TypeEnum)
									  {
										  //double2int
										  case _Int:
											  return newDouble((double)((langInt)obj1)->getInt());
										  case _Double:
											  return newDouble((double)((langDouble)obj1)->getDouble());
									  }
									  break;
								  case _String:
									  switch (obj1->type->TypeEnum)
									  {
										  //double2int
										  case _String:
											  return obj1;
									  }
									  break;
							  }

							  throw langRuntimeException((std::string(obj1->type->name) + "as " + obj2->type->name + "出来ない").c_str());
				}
				case _Class:

					break;
		}
		throw langRuntimeException((std::string(obj1->type->name) + "as " + obj2->type->name + "出来ない").c_str());
	}

}
