#ifndef __DAG__H__
#define __DAG__H__//#pragma once
#include <vector>
#include "parseObj.h"
#include "lang.h"
namespace lang
{
    namespace plugin
    {
		class dagger
        {
        std::vector<parseObj*>& tokens;
        public:
			dagger(std::vector<parseObj*>& p);
			~dagger(void);
        };
    }
}
#endif
