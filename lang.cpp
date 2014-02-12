// lang.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//
#pragma once
#include "stdafx.h"
#include "parser.h"
#include "scope.h"
#include <memory>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include "langException.h"
#include <cassert>
#include <thread>
//#include "parseObj.h"
//#include "parserEnum.h"
namespace lang
{
langObject NULLOBJECT = newObject(nullptr);
}
#pragma once
    char* enumtable[]={"identifier","num","doublequote","str","leftparent","rightparent","comma","plus","minus","multiply","equal","equalequal","semicolon","blockstart","blockend","plusplus","minusminus","greater","less","greaterequal","lessequal","modulo","plusequal","minusequal","dot","division","leftbracket","rightbracket","debbug_stop","multiplyequal","divisionequal","moduloequal","or","oror","and","andand","xor","notequal","not","notnot","leftshift","rightshift","leftshiftequal","rightshiftequal","andequal","orequal","xorequal","chr","none"
    ,"_class"
    ,"_new",
    "_this",};
    
    char* parserEnumToString(lang::parserEnum i)
    {
        return enumtable[i];
    }
bool lang::gc_view  = false;
extern int lang::error_level;
    //#define out auto
    v�i�f�ցf�jv
    v�i�f�ցf�jv
    v�i�f�ցf�jv
std::string �Ă��k�h�m�d����Ă�H(std::string input,int index)
{
    int i, j;
    for(i = index;i>0;i--)
    {
        if(input[i] == '\n') break;
    }
    for(j = index;j<input.size();j++)
    {
        if(input[j] == '\n') break;
    }
    return input.substr(i + 1,j-i - 1);
}
enum class option
{
    none,errorlevel,
};
int _tmain(int argc, _TCHAR* argv[])
{
    option o = option::none;
    lang::error_level = 0;
    bool ahogc = false,parserresult = false;
    for(int i = 1;i < argc;i++)
    {
        switch (o)
        {
        case option::none:
        if(!_tcscmp(argv[i],L"-ahogc"))
        {
            ahogc = true;
        }
        else
        if(!_tcscmp(argv[i],L"-parserresult"))
        {
            parserresult = true;
        }
        else
        if(!_tcscmp(argv[i],L"-gcview"))
        {
            lang::gc_view = true;
        }
        else
        if(!_tcscmp(argv[i], L"-errorlevel"))
        {
            o = option::errorlevel;
        }
            break;
        case option::errorlevel:
            lang::error_level = _ttoi(argv[i]);
            o = option::none;
            break;
        default:
            break;
        }
    }
	std::cout<<"???"<<�Ă��k�h�m�d����Ă�H("a\nbc\n",3);lang::NULLOBJECT = new lang::Object();
    lang::NULLOBJECT->type->name = "null";
	while (std::getchar())
	{
		std::string input;
        std::stringstream ss;
		//std::getline(std::cin,input);
		std::ifstream ifs( "test.txt" );
        while(ifs && getline(ifs, input)) {
            ss << input << '\n';
        }
        input = ss.str();
        std::shared_ptr<lang::parser> pars;
        try
        {
          pars=std::make_shared< lang::parser>(*new std::string(input));//�A�E�g
		}
        catch(lang::langParseException ex)
        {
            std::cout<<std::endl<<"lang::langParseException - lang::parser"<<std::endl<<ex.what();
            continue;
        }
        //out testobj=new lang::parseObj("hoge");//�A�E�g
		//std::cout<<pars->program<<std::endl<<testobj->getString()<<std::endl;*pars->parsers[i]->toString()*/
        int nest = 0;
        if(parserresult)
        for(int i=0;i<pars->parsers.size();i++)
        {
            if(pars->parsers[i]->pEnum == lang::blockend)nest--;
            if(pars->parsers[i]->pEnum == lang::rightparent)nest--;
            std::cout<<i<<"\t";
            for(int j=0;j<nest;j++)std::cout<<" ";
            std::cout<<input.substr(pars->parsers[i]->sourcestartindex, pars->parsers[i]->sourceendindex - pars->parsers[i]->sourcestartindex + 1)<<"\t"<<parserEnumToString(pars->parsers[i]->pEnum)<<std::endl;
            if(pars->parsers[i]->pEnum == lang::blockstart)nest++;
            if(pars->parsers[i]->pEnum == lang::leftparent)nest++;
        }
        try
        {
    v�i�f�ցf�jv
    v�i�f�ցf�jv
    v�i�f�ցf�jv
        //#if AHO_GC //�X���b�h�œ���������A�z��GC
            if(ahogc)std::thread thd([]{ while (true) lang::gc->start();});
        //#endif
           // lang::NULLOBJECT = new lang::Object();
            pars->runner->run();

            std::cout<<"���s�I �ϐ���萔���폜"<<std::endl;
        }
        catch(lang::langRuntimeException ex)
        {
            std::cout << std::endl << "lang::langRuntimeException - lang::scope::run" << std::endl << ex.what() << std::endl << "�ꏊ?:" << std::endl;
            for(auto i : ex.stacktrace)
            {
                std::cout << �Ă��k�h�m�d����Ă�H(input,ex.tokens[i.first]->sourcestartindex);
                                //std::cout << input.substr(ex.tokens[i.first]->sourcestartindex, ex.tokens[i.second]->sourceendindex - ex.tokens[i.first]->sourcestartindex + 1) << std::endl;
                break;
            }
            std::cout << "StackTrace" << std::endl;
            for(auto i : ex.funcstacktrace)
            {
                std::cout << i << std::endl;
            }
            std::cout<<"�ُ�I�� �ϐ���萔���폜"<<std::endl;
        }
        catch(std::exception ex)
        {
            std::cout<<"BUG!!!"<<ex.what()<<std::endl;
            std::cout<<"�ُ�I�� �ϐ���萔���폜"<<std::endl;
        }
        catch(...)
        {
            std::cout<<"BUG!!!!!!!!!!!!!!!!!!"<<std::endl;
            std::cout<<"�ُ�I�� �ϐ���萔���폜"<<std::endl;
        }
        lang::gc->roots.clear();
        lang::gc->root->variable._variable.clear();
        lang::gc->start();
        std::cout<<"�萔�̍폜"<<std::endl;
        for(int i=0;i<pars->parsers.size();i++)
        {
            delete pars->parsers[i]->ptr;
            delete pars->parsers[i];
        }
        //(new lang::scope(pars->parsers))->run();
        //}
        //catch(char* ex)
        //{
        //    std::cout<< ex<<std::endl;
        //}
		//delete pars;
	}
	return 0;
}
