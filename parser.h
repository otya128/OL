#pragma once
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
        std::vector<std::tuple<int,std::string&,std::string> > staticmemberevals;
#else
        std::vector<std::pair<int,std::pair<std::string&,std::string> > > staticmemberevals;
#endif
        void parser::function();
        void parser::namespaceparse();
        void parser::staticparse();
    public:
        scope* runner;
        std::vector<parseObj*> parsers;
        std::vector<std::string> usings;
        std::string program;
        parser(std::string);
        ~parser(void);
    };
}
