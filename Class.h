#ifndef __LANG_CLASS_H__//#pragma once
#define __LANG_CLASS_H__
#include "lang.h"
#include "Object.h"
#include "variable.h"
namespace lang
{
	class scope;
#define MEMBERTYPEITEM(x,y,z) membertypeitem(x, std::pair<langObject, qualifier>(y,z)) 
	typedef std::pair<std::string, std::pair<langObject, qualifier> > membertypeitem;
	typedef std::vector<membertypeitem> membertype_;
	typedef membertype_* membertype;
  	typedef std::vector<membertypeitem>::iterator memit;
  	typedef membertypeitem mempr;
	class Class : public Object
	{
	protected:
		Class(Class* clas);
	public:
		membertype member;
		langFunction finalize;
		Class(std::string name, int index, membertype member, scope* scope, membertype staticmember);
		virtual ~Class(void);
		//���O
		std::string name;
		lang::scope* scope;
		int index;
		virtual std::string toString();
		//�X�R�[�v
		lang::scope* thisscope;
		Class* base;
		virtual bool trygetMember(std::string& name, langObject& obj, lang::scope *access);
		virtual langObject getMember(std::string& name, lang::scope *access);
		virtual langObject setMember(std::string& name, langObject obj, lang::scope *access);
	};
	class ClassObject : public Class
	{
	public:
		Class* staticClass;
		virtual std::string toString(void);
		ClassObject(Class* type);
		~ClassObject(void);
	};
}
#endif
