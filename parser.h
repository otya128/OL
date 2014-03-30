#ifndef __LANG_PARSER_H__//#pragma once
#define __LANG_PARSER_H__
#include <vector>
#include "parseObj.h"
#include "scope.h"
#include "GC.h"
namespace lang
{
	template <class T>
	class stack : public std::vector<T>
	{
	public:
		void push(const T &__x);
		void pop();
		T top();
	};
	//extern ‚ª‚×‚±‚ê* gc;
	enum ENUMCLASS sts
	{
		Empty,      //   = 0,
		Func,       //   = 1,
		stsClass,   //   = 2,
		NameSpace,  //   = 4,
	};
	struct BlockStruct
	{
		sts type;
		std::string name;
		BlockStruct(sts s, std::string n);
	};
	class parser
	{
	private:
		std::vector<int> staticevals;
#ifdef CPP11
		std::vector<std::tuple<int, std::string&, std::string> > staticmemberevals;
#else
		std::vector<std::pair<int,std::pair<std::string&,std::string> > > staticmemberevals;
#endif
		void function();
		void function2(int &i, int endstacksize, lang::stack<BlockStruct> &funcStack, std::string &namesp);
		Property* propertyparse(int &i, int endstacksize, lang::stack<BlockStruct> &funcStack, std::string &namesp);
		void namespaceparse();
		void staticparse();
		void lambdaparse(int index);
		void conditionalparse(int index);
		void catchparse(int index);
	public:
		scope* runner;
		std::vector<parseObj*> parsers;
		std::vector<std::string> usings;
		std::vector < std::pair<int, langClass>> extendslist;
		std::string program;
		parser(std::string);
		void WARNING(const char* param, int level = 0);
		int errcount;
		~parser(void);
		langObject Run();
	};
}
#endif
