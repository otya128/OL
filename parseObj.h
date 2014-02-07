#pragma once
#include"parserEnum.h"
#include"Object.h"
#include <memory>
namespace lang
{
class parseObj
{
private:
public:
	std::shared_ptr<Object> ptr;
	std::string* name;
	parserEnum pEnum;
	int getInt();
	std::string getString();
	double getDouble();
	char getChar();
	parseObj(parserEnum p,std::string* n);
	parseObj(int obj);
	parseObj(std::string obj);
	parseObj(double obj);
	parseObj(char obj);
	~parseObj(void);
    std::string toString(void);
};
}

