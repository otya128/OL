#pragma once
#include "parserEnum.h"
#include "Object.h"
#include <memory>
namespace lang
{
    class parseObj
    {
    private:
    public:
        int sourcestartindex;
        int sourceendindex;
        langObject ptr;
        std::string* name;
        parserEnum pEnum;
        int getInt();
        std::string getString();
        double getDouble();
        char getChar();
        parseObj(parserEnum p,std::string* n,int i,int j);
        parseObj(int obj,int i,int j);
        parseObj(std::string obj,int i,int j);
        parseObj(double obj,int i,int j);
        parseObj(char obj,int i,int j);
        ~parseObj(void);
        std::string toString(void);
    };
}

