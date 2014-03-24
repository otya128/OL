#ifndef __LANG_PARSER_H__//#pragma once
#define __LANG_PARSER_H__
#include <vector>
#include "parseObj.h"
#include "scope.h"
#include "GC.h"
namespace lang
{
	//extern ‚ª‚×‚±‚ê* gc;
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
		void namespaceparse();
		void staticparse();
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
