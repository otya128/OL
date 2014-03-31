#pragma once
#include "stdafx.h"
#include "parseObj.h"
#include "GC.h"
//#include "parserEnum.h"
//#include "Object.h"
namespace lang
{
    parseObj::parseObj(int obj,int i,int j)
    {
        this->name = nullptr;
        this->sourceendindex = j;
        this->sourcestartindex = i;
        this->ptr = newInt(obj);
        this->pEnum=num;
    }
    parseObj::parseObj(std::string obj,int i,int j)
    {
        this->name = nullptr;
        this->sourceendindex = j;
        this->sourcestartindex = i;
        /*auto str=new std::string(obj);
        this->ptr=(void*)(str->c_str());
        str=nullptr;*/
        //this->ptr=(void*)(obj.copy().data());
        this->ptr = newString(&obj);
        this->pEnum=str;
    }

    parseObj::parseObj(double obj,int i,int j)
    {
        this->sourceendindex = j;
        this->sourcestartindex = i;
		this->ptr = newDouble(obj);
		this->pEnum = num;
		this->name = nullptr;
    }

    parseObj::parseObj(char obj,int i,int j)
    {
        this->sourceendindex = j;
        this->sourcestartindex = i;
        this->ptr = new Char((char)obj);
        //this->ptr=new char(obj);
        this->pEnum=chr;
        this->name = nullptr;
    }
    parseObj::parseObj(wchar obj,int i,int j)
    {
        this->sourceendindex = j;
        this->sourcestartindex = i;
        this->ptr = new WChar((wchar)obj);
        this->name = nullptr;
        //this->ptr=new char(obj);
        this->pEnum=chr;
    }

    parseObj::parseObj(parserEnum p,std::string* n,int i,int j)
    {
        this->sourceendindex = j;
        this->sourcestartindex = i;
        this->pEnum = p;
        this->name  = n;
        this->ptr = nullptr;
    }
    parseObj::parseObj(parserEnum p,std::string* n,int i,int j, langObject obj)
    {
        this->sourceendindex = j;
        this->sourcestartindex = i;
        this->pEnum = p;
        this->name  = n;
        this->ptr   = obj;
    }

    parseObj::~parseObj(void)
    {
		if (!(this->pEnum >= _true && this->pEnum <= _null))
		{
			if (lang::gc->object.find(this->ptr) != lang::gc->object.end())
				delete this->ptr;
			lang::gc->uncontroll(this->ptr);
		}
		delete this->name;
    }

    int parseObj::getInt(void)
    {
        if(this->pEnum==num)
        {
            if(this->ptr->type->TypeEnum==_Int)
                return (static_cast<Int*>(this->ptr/*.get()*/))->getInt();
            //return (static_cast<std::shared_ptr<Int>>(this->ptr))->getInt();
        }
        //throw std::exception::exception("�^int�ɃL���X�g�s��");
    }
    std::string parseObj::getString(void)
    {
        if(this->pEnum==str)
        {
            if(this->ptr->type->TypeEnum==_String)
                return *((static_cast<String*>(this->ptr/*.get()*/))->getString());
            //return *((static_cast<std::shared_ptr<String>>(this->ptr))->getString());
            //return (char*)ptr;//(std::string)(((char*)ptr)+4);
        }
        //throw std::exception::exception("�^string�ɃL���X�g�s��");
    }

    double parseObj::getDouble(void)
    {
        if(this->pEnum==num)
        {
        return *((double*)this->ptr);
        }
        //throw std::exception::exception("�^double�ɃL���X�g�s��");
    }
    char parseObj::getChar(void)
    {
     if(this->pEnum==chr)
     {
     return *(char*)this->ptr;
     }
        //throw std::exception::exception("�^char�ɃL���X�g�s��");
    }
    std::string parseObj::toString(void)
    {
        if(this->ptr == nullptr)
        {
            return *this->name;
        }
        return this->ptr->toString();
	}
	Conditional::Conditional(int c, int e)
	{
		this->colonindex = c;
		this->endindex = e;
		this->_isconditional = 0x10000FF6;
		this->type = NULLOBJECT->type;
	}

	Conditional::~Conditional()
	{
	}
	bool Conditional::isconditional()
	{
		return this->_isconditional == 0x10000FF6;
	}
	Catcher::Catcher()
	{
		this->_iscatcher = 0x10000FF4;
		this->type = NULLOBJECT->type;
	}
	Catcher::~Catcher()
	{

	}
	bool Catcher::iscatcher()
	{
		return this->_iscatcher == 0x10000FF4;
	}
	void Catcher::Add(std::string *type, std::string *varname, int index)
	{
		Catcherc cc;
		cc.type = type;
		cc.varname = varname;
		cc.index = index;
		this->Catchers.push_back(cc);
	}
}
