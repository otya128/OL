#pragma once
#include "stdafx.h"
#include "variable.h"
#include "Function.h"
#include "langException.h"
#include "Class.h"
//#include "Object.h"
namespace lang{
	variable::variable(variable* s,scope *o)
    {
		this->_variable = std::map<std::string, std::pair<langObject, qualifier> >();
        this->childVariable = nullptr;
        this->parentVariable = s;
        s->childVariable = this;
		this->owner = o;
    }


	variable::variable(scope *o)
    {
        //this->_variable = std::map<std::string,langObject>();
        this->childVariable = nullptr;
		this->parentVariable = nullptr;
		this->owner = o;
    }
    variable::~variable(void)
    {
        if(this->childVariable != nullptr)
            this->childVariable->parentVariable = nullptr;
    }

	langObject variable::search(std::string name, scope *access)
    {
        if(this->_variable.find(name) != this->_variable.end())
            return this->_variable[name].first;
        else
        {
            //lang::NULLOBJECT
            if(this->parentVariable == nullptr) return lang::NULLOBJECT;
			return this->parentVariable->search(name, access);
        }
    }
	bool variable::definedVar(std::string name, scope *access)
    {
        if(this->_variable.find(name) != this->_variable.end())
            return true;
        else
        {
            //lang::NULLOBJECT
            if(this->parentVariable == nullptr) return false;
			return this->parentVariable->definedVar(name, access);
        }
    }
	langObject variable::operator()(std::string name, scope *access)
    {
		varitf f = this->_variable.find(name);
		if (f != this->_variable.end())
		{
			if (f->second.first && f->second.first is _Property)
			{
				return ((Property*)f->second.first)->Get(this,access);
			}
			qualifier q = f->second.second;//this->_variable[name];
			if (q & private_)
			{
				if (!access->_this || this->owner->_this->thisscope != access->_this->thisscope)
				{
					if (access->_this && (q & protected_))
					{
						langClass base = access->_this->base;
						while (base)
						{
							if (this->owner->_this == base)
								return f->second.first;
							base = base->base;
						}
						throw_langRuntimeException("protected member %sにはアクセスできません", name.c_str());
					}
					throw_langRuntimeException("private member %sにはアクセスできません", name.c_str());
				}
			}
			return f->second.first;//this->_variable[name];
		}
		else if (this->parentVariable != nullptr)return this->parentVariable->search(name, access);
        return lang::NULLOBJECT;
    }
	langObject variable::set(std::string name, langObject object, scope *access)
	{
		varitf f = this->_variable.find(name); 
		if (object && object is _Function && f != this->_variable.end() && f->second.first is _Function)
		{
			qualifier q = f->second.second;
			if (q & const_)
			{
				throw_langRuntimeException("const %sに代入できません", name.c_str());
			}
			if (q & private_)
			{
				if (!access->_this || this->owner->_this->thisscope != access->_this->thisscope)
				{
					if (access->_this && (q & protected_))
					{
						langClass base = access->_this->base;
						while (base)
						{
							if (this->owner->_this == base)
								return f->second.first;
							base = base->base;
						}
						throw_langRuntimeException("protected member %sにはアクセスできません", name.c_str());
					}
					throw_langRuntimeException("private member %sにはアクセスできません", name.c_str());
				}
			}
			langFunction f1 = (langFunction)f->second.first;
			if (f1->isoverload())
			{
				((OverloadFunction*)f1)->functions.push_back((langFunction)object);
				return f1;
			}
			OverloadFunction* of = new OverloadFunction(f1, (langFunction)object);
			this->_variable[name].first = of;// = //std::pair<langObject, qualifier>(of, f->second.second);
			return of;//this->_variable[name];
		}
        if(f != this->_variable.end())
		{
			if (f->second.first && f->second.first is _Property)
			{
				return ((Property*)f->second.first)->Set(object, this, access);
			}
			qualifier q = f->second.second;
			if (q & const_)
			{
				throw_langRuntimeException("const %sに代入できません",name.c_str());
			}
			if (q & private_)
			{
				if (!access->_this || this->owner->_this->thisscope != access->_this->thisscope)
				{
					if (access->_this && (q & protected_))
					{
						langClass base = access->_this->base;
						while (base)
						{
							if (this->owner->_this == base)
								return f->second.first;
							base = base->base;
						}
						throw_langRuntimeException("protected member %sにはアクセスできません", name.c_str());
					}
					throw_langRuntimeException("private member %sにはアクセスできません", name.c_str());
				}
			}
			this->_variable[name].first = object;// = std::pair<langObject, qualifier>(object,f->second.second);
			return object;
		}
        else
        {
            //lang::NULLOBJECT
            if(this->parentVariable == nullptr) return nullptr;
			return this->parentVariable->set(name, object, access);
        }
    }
    void variable::add(std::string name,langObject object,qualifier q)
	{
		varitf f = this->_variable.find(name);
		if (object is _Function && (f) != this->_variable.end() && f->second.first is _Function)
		{
			if (f->second.second != q)
			{
				throw langRuntimeException("オーバーロードされた関数とアクセスレベルが違います。[未実装]");
			}
			langFunction f1 = (langFunction)f->second.first;
			if (f1->isoverload())
			{
				((OverloadFunction*)f1)->functions.push_back((langFunction)object);
				return;
			}
			this->_variable[name] = std::pair<langObject, qualifier>(new OverloadFunction(f1, (langFunction)object),q);
			return;
		}
		this->_variable[name] = std::pair<langObject, qualifier>(object, q);
		//this->typequalifier[name] = q;
    }
}
