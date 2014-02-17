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
