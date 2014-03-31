#ifndef __LANG_VARIABLE_H__
#define __LANG_VARIABLE_H__//#pragma once
#include <map>
#include <memory>
#include "Object.h"
//#include "scope.h"
namespace lang
{
	class scope;
	enum qualifier
	{
		public_ = 0,//default public
		private_ = 1,
		protected_ = 3,
		const_ = 4,
	};
  typedef std::map<std::string, std::pair<langObject, qualifier> >::iterator varit;
    typedef std::pair<std::string, std::pair<langObject, qualifier> > varpr;
  typedef std::map<std::string, std::pair<langObject, qualifier> >::iterator varitf;
	class variable
	{
	public:
		std::map<std::string, std::pair<langObject, qualifier> > _variable;
		//std::map<std::string, qualifier> typequalifier;

		variable *parentVariable;
		variable *childVariable;
		variable(variable* s, scope *o);
		variable(scope *o);
		~variable(void);
		bool definedVar(std::string name, scope *access);
		langObject search(std::string name, scope *access);
		langObject operator()(std::string name, scope *access);
		langObject set(std::string name, langObject object, scope *access);
		void add(std::string name, langObject object, qualifier q = public_);
		scope *owner;
		/*langObject operator=(std::string name)
		{
		if(this->_variable.find(name) != this->_variable.end())
		return this->_variable[name];
		else if(this->parentVariable != nullptr)return this->parentVariable->setsearch(name);
		}*/
	};
}
#endif
