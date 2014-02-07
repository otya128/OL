#pragma once
#include <vector>
#include "parseObj.h"
namespace lang
{
class parser
{
public:
	std::vector<parseObj*> parsers;
	std::string program;

public:
    parser(std::string);
public:
    ~parser(void);
};
}
