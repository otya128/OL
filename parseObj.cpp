#pragma once
#include "stdafx.h"
#include "parseObj.h"
//#include "parserEnum.h"
//#include "Object.h"
namespace lang
{
parseObj::parseObj(int obj)
{
	this->ptr=std::make_shared<Int>(obj);
	this->pEnum=parserEnum::num;
}
parseObj::parseObj(std::string obj)
{
	/*auto str=new std::string(obj);
	this->ptr=(void*)(str->c_str());
	str=nullptr;*/
	//this->ptr=(void*)(obj.copy().data());
	this->ptr=std::make_shared<String>(&obj);
	this->pEnum=parserEnum::str;
}

parseObj::parseObj(double obj)
{
	//this->ptr=new double(obj);
	//this->pEnum=parserEnum::num;
}

parseObj::parseObj(char obj)
{
	//this->ptr=new char(obj);
	//this->pEnum=parserEnum::chr;
}

parseObj::parseObj(parserEnum p,std::string* n)
{
	this->pEnum = p;
	this->name  = n;
}

parseObj::~parseObj(void)
{
	//delete this->ptr;
    delete this->name;
}

int parseObj::getInt(void)
{
	if(this->pEnum==parserEnum::num)
	{
        if(this->ptr->type->TypeEnum==PreType::_Int)
        return (static_cast<Int*>(this->ptr.get()))->getInt();
            //return (static_cast<std::shared_ptr<Int>>(this->ptr))->getInt();
	}
	throw std::exception::exception("型intにキャスト不可");
}
std::string parseObj::getString(void)
{
	if(this->pEnum==parserEnum::str)
	{
        if(this->ptr->type->TypeEnum==PreType::_String)
        return *((static_cast<String*>(this->ptr.get()))->getString());
            //return *((static_cast<std::shared_ptr<String>>(this->ptr))->getString());
		//return (char*)ptr;//(std::string)(((char*)ptr)+4);
	}
	throw std::exception::exception("型stringにキャスト不可");
}

double parseObj::getDouble(void)
{
	/*if(this->pEnum==parserEnum::num)
	{
		return *((double*)this->ptr);
	}*/
	throw std::exception::exception("型doubleにキャスト不可");
}
char parseObj::getChar(void)
{/*
	if(this->pEnum==parserEnum::chr)
	{
		return (char)this->ptr;
	}*/
	throw std::exception::exception("型charにキャスト不可");
}
std::string parseObj::toString(void)
{
    if(this->ptr==nullptr)
    {
        return *this->name;
    }
    return this->ptr->toString();
}
}
