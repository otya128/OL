#pragma once
#include <vector>
#include "parseObj.h"
#include "lang.h"
namespace lang
{
    namespace plugin
    {
        class õ
        {
        std::vector<parseObj*>& tokens;
        public:
            õ(std::vector<parseObj*>& p);
            ~õ(void);
        };
    }
}
