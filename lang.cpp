// lang.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//
#pragma once
#include "stdafx.h"
#include "parser.h"
#include "scope.h"
#include <memory>
//#include "parseObj.h"
//#include "parserEnum.h"

#pragma once
    char* enumtable[]={"identifier","num","doublequote","str","leftparent","rightparent","comma","plus","minus","multiply","equal","equalequal","semicolon","blockstart","blockend","plusplus","minusminus","greater","less","greaterequal","lessequal","modulo","plusequal","minusequal","dot","division","leftbracket","rightbracket","debbug_stop","multiplyequal","divisionequal","moduloequal","or","oror","and","andand","xor","notequal","not","notnot","leftshift","rightshift","leftshiftequal","rightshiftequal","andequal","orequal","xorequal","chr","none"};
    
    char* parserEnumToString(lang::parserEnum i)
    {
        return enumtable[i];
    }
    #define out auto
int _tmain(int argc, _TCHAR* argv[])
{
	std::cout<<"???";
	while (true)
	{
		std::string input;
		std::getline(std::cin,input);
		//try
        //{
        out pars=std::make_shared< lang::parser>(*new std::string(input));//アウト
		//out testobj=new lang::parseObj("hoge");//アウト
		//std::cout<<pars->program<<std::endl<<testobj->getString()<<std::endl;
        for(int i=0;i<pars->parsers.size();i++)
        {
            std::cout<<i<<"\t"<<pars->parsers[i]->toString()<<"\t"<<parserEnumToString(pars->parsers[i]->pEnum)<<std::endl;
        }
        (new lang::scope(pars->parsers))->run();
        //}
        //catch(char* ex)
        //{
        //    std::cout<< ex<<std::endl;
        //}
		//delete pars;
	}
	return 0;
}
