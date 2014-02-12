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
#include <windows.h>
//#include "parseObj.h"
//#include "parserEnum.h"
namespace lang
{
 Type* ObjectType = new Type(PreType::_Object);
langObject NULLOBJECT = nullptr;//ewObject(nullptr);
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
int hook(int a1, char *a2, int *a3)
{
    std::cout<<a2;
    return a1;
}
int _tmain(int argc, _TCHAR* argv[])
{
//_CrtSetReportMode( 1, _CRTDBG_MODE_WNDW );
    option o = option::none;
    lang::error_level = 0;
    bool ahogc = false,parserresult = false, leakcheck = false, pause = false;
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
        if(!_tcscmp(argv[i],L"-leakcheck"))
        {
            leakcheck = true;
        }
        else
        if(!_tcscmp(argv[i],L"-pause"))
        {
            pause = true;
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
    if(leakcheck) _CrtSetReportHook((_CRT_REPORT_HOOK)hook);
	std::cout<<"???"<<�Ă��k�h�m�d����Ă�H("a\nbc\n",3);lang::NULLOBJECT = new lang::Object();
    lang::NULLOBJECT->type->name = "null";
	while (true)//std::getchar())
	{
		std::string input;
        std::stringstream ss;
		//std::getline(std::cin,input);
		std::ifstream ifs( "test.txt" );
        while(ifs && getline(ifs, input)) {
            ss << input << '\n';
        }
        input = ss.str();
        lang::parser* pars;
        try
        {
          pars=new lang::parser(input);//�A�E�g
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
           __try
           {
            pars->runner->run();
           }
           __except(EXCEPTION_EXECUTE_HANDLER){
                _EXCEPTION_POINTERS *info = GetExceptionInformation();
           }
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
        std::vector<lang::scope*> del;
        for(auto i : lang::gc->roots)
        {
            del.push_back(i.first);
        }
        lang::gc->roots.clear();
        //lang::gc->root->variable._variable.clear();
        lang::gc->start();
        for(int i=0;i<del.size();i++)
        {
            delete del[i];
        }
        std::cout<<"�萔�̍폜"<<std::endl;
        for(int i=0;i<pars->parsers.size();i++)
        {
           // delete pars->parsers[i]->ptr;
            delete pars->parsers[i];
        }
        delete pars;
        //(new lang::scope(pars->parsers))->run();
        //}
        //catch(char* ex)
        //{
        //    std::cout<< ex<<std::endl;3333
        //}
		//delete pars;
        break;
	}
    delete lang::NULLOBJECT;
    delete lang::ObjectType;
        if(leakcheck) 
        {
            _CrtDumpMemoryLeaks();
        }
        if(pause)
        #if defined(_WIN32)
            system("PAUSE");//WINDOWS�Ȃ炱�����̕����e�؂�����g��
        #elif
            std::getchar();
        #endif
	return 0;
}
