// lang.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
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
#include "Function.h"
#include <time.h>
#include "†.h"
#ifndef _DEBUG
#include <eh.h>
//構造化例外が発生すると、この関数が呼ばれる
void se_translator_function(unsigned int code, struct _EXCEPTION_POINTERS* ep)
{
    throw ep; //標準C++の例外を発生させる
}
#endif
#define END goto theend
#define FAILEND result = 1;goto theend
//#include "parseObj.h"
//#include "parserEnum.h"
namespace lang
{
    namespace plugin
    {

    }
    bool running;
    namespace lib{void init();}
    Type* ObjectType = new Type(PreType::_Object);
    langObject NULLOBJECT = nullptr;//ewObject(nullptr);
    bool ahogc = false,parserresult = false, leakcheck = false, pause = false;
#if _DEBUG
    std::vector<int> BreakPoint;//行で指定
#endif
    typedef langObject (*BuiltFunc)(std::vector<langObject>);
    extern std::map<std::string,BuiltFunc>* BuiltFunction;
}
using namespace lang;
#pragma once
char* enumtable[]={"identifier","num","doublequote","str","leftparent","rightparent","comma","plus","minus","multiply","equal","equalequal","semicolon","blockstart","blockend","plusplus","minusminus","greater","less","greaterequal","lessequal","modulo","plusequal","minusequal","dot","division","leftbracket","rightbracket","debbug_stop","multiplyequal","divisionequal","moduloequal","or","oror","and","andand","xor","notequal","not","notnot","leftshift","rightshift","leftshiftequal","rightshiftequal","andequal","orequal","xorequal","chr","none",
    "_class",
    "_new",
    "_this",
    "_namespace",
    "_using",
    "_static",
};

char* parserEnumToString(lang::parserEnum i)
{
    return enumtable[i];
}
char* escapetable[256];
std::string escape(std::string& str)
{
    std::string result;
    int size = str.length();
    for(int i=0;i<size;i++)
    {
        result.append(escapetable[(unsigned char)str.at(i)]);
    }
    return result;
}
bool lang::gc_view  = false;
extern int lang::error_level;
//#define out auto
v（’ω’）v
    v（’ω’）v
    v（’ω’）v
    std::string てかＬＩＮＥやってる？(std::string input,int index)
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
int _tmain(int argc, _TCHAR *argv[])
{
    wchar_t et[256];
    for(unsigned char i=0;i<255;i++)
    {
        wchar_t a;
        (&a)[0] = i;
        (&a)[1] = '\0';
        et[(unsigned char)i] = i;
        escapetable[(unsigned char)i] = (char*)&(et[(unsigned char)i]);
    }
    escapetable[10] = "\\n";
    escapetable[13] = "\\r";
    //([](int x){return x*x;})();
#ifndef _DEBUG
    //スレッド毎に変換関数を登録する
    _set_se_translator(se_translator_function);
#endif
    //_CrtSetReportMode( 1, _CRTDBG_MODE_WNDW );
    _CrtSetBreakAlloc(223);_CrtSetBreakAlloc(221);
    _CrtSetBreakAlloc(218);
    option o = option::none;
    lang::error_level = 0;
    bool endpause= false,json = false;
    int result = 0;
    _TCHAR*filename = L"";
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
                                if(!_tcscmp(argv[i],L"-endpause"))
                                {
                                    endpause = true;
                                }
                                else
                                    if(!_tcscmp(argv[i],L"-json"))
                                    {
                                        json = true;
                                    }
                                    else
                                        if(!_tcscmp(argv[i], L"-errorlevel"))
                                        {
                                            o = option::errorlevel;
                                        }
                                        else
                                        {
                                            filename = argv[i];
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
    if(!json)
#if _DEBUG
        std::cout<<"OtyaLanguage DEBUG build"<<std::endl;
#else
        std::cout<<"OtyaLanguage"<<std::endl;
#endif
    //lang::gc_view = true;
    if(leakcheck) _CrtSetReportHook((_CRT_REPORT_HOOK)hook);
    lib::init();
    lang::NULLOBJECT = new lang::Object();
    lang::NULLOBJECT->type->name = "null";
    while (true)//std::getchar())
    {
        std::string input;
        std::stringstream ss;
        //std::getline(std::cin,input);
        std::ifstream ifs( filename);
        if(ifs.fail())
        {
            _tcprintf(L"file:%sを開けません\n",filename);//std::cout<<"file:"<<filename<<"を開けません"<<std::endl;
            v（’ω’）v ;v（’ω’）v ;FAILEND;v（’ω’）v v（’ω’）v 
        }
        while(ifs && getline(ifs, input)) {
            ss << input << '\n';
        }
        input = ss.str();
        lang::parser* pars;
        try
        {
            pars = new lang::parser(input);//アウト
        }
        catch(lang::langParseException ex)
        {
            std::cout<<std::endl<<"lang::langParseException - lang::parser"<<std::endl<<ex.what();
            continue;
        }
        catch(lang::langRuntimeException ex)
        {
            std::cout << std::endl << "lang::langRuntimeException - lang::scope::run" << std::endl << ex.what() << std::endl << "場所?:" << std::endl;
            for(auto i : ex.stacktrace)
            {
                std::cout << てかＬＩＮＥやってる？(input,ex.tokens[i.second]->sourcestartindex);
                //std::cout << input.substr(ex.tokens[i.first]->sourcestartindex, ex.tokens[i.second]->sourceendindex - ex.tokens[i.first]->sourcestartindex + 1) << std::endl;
                //break;
            }
            std::cout << "StackTrace" << std::endl;
            for(auto i : ex.funcstacktrace)
            {
                std::cout << i << std::endl;
            }
            std::cout<<"異常終了 変数や定数を削除"<<std::endl;
            std::getchar();
            v（’ω’）v ;v（’ω’）v ;FAILEND;v（’ω’）v v（’ω’）v //continue;
        }
        for(int i=0;i<pars->parsers.size();i++)
        {
            if(pars->parsers[i]->ptr != nullptr)
            {
                lang::gc->constroot.push_back(pars->parsers[i]->ptr);
            }
        }
        //out testobj=new lang::parseObj("hoge");//アウト
        //std::cout<<pars->program<<std::endl<<testobj->getString()<<std::endl;*pars->parsers[i]->toString()*/
        int nest = 0;
        if(json)
        {
            std::cout << "{\"parser\":[";
            for(auto &&i : pars->parsers)
            {
                std::cout << '{' << "\"name\":\"" << (i->name ? escape(*i->name) : "") << '\"' <<',' << "\"pEnum\":\"" << enumtable[i->pEnum] << '\"' << ',' << "\"ptr\":";
                if(i->ptr)
                {
                    if(i->ptr is _Int)
                    {
                        std::cout << *(int*)i->ptr->getPointer();
                    }
                    else
                        if(i->ptr is _String)
                        {
                            std::cout << '\"' << escape(*(std::string*)i->ptr->getPointer()) << '\"';
                        }
                }
                else
                {
                    std::cout << "null";
                }
            std::cout << '}' << ',';
            }
            std::cout << ']' << ',';
            std::cout << '}';
            exit(0);
        }
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
                v（’ω’）v
                    v（’ω’）v
                    v（’ω’）v
                    //#if AHO_GC //スレッドで動き続けるアホなGC
                    if(ahogc)std::thread thd([]{ while (true) lang::gc->start();});
                //#endif
                // lang::NULLOBJECT = new lang::Object();
                running = true;
                pars->runner->run();
                std::cout<<"実行終 変数や定数を削除"<<std::endl;
            }
            catch(lang::langRuntimeException ex)
            {
                std::cout << std::endl << "lang::langRuntimeException - lang::scope::run" << std::endl << ex.what() << std::endl << "場所?:" << std::endl;
                for(auto i : ex.stacktrace)
                {
                    std::cout << てかＬＩＮＥやってる？(input,ex.tokens[i.second]->sourcestartindex);
                    //std::cout << input.substr(ex.tokens[i.first]->sourcestartindex, ex.tokens[i.second]->sourceendindex - ex.tokens[i.first]->sourcestartindex + 1) << std::endl;
                    //break;
                }
                std::cout << "StackTrace" << std::endl;
                for(auto i : ex.funcstacktrace)
                {
                    std::cout << i << std::endl;
                }
                std::cout<<"異常終了 変数や定数を削除"<<std::endl;
            }
#if !defined(_DEBUG)//デバッグならデバッガの機能を利用する
            catch(std::exception ex)
            {
                std::cout<<"BUG!!!"<<ex.what()<<std::endl;
                std::cout<<"異常終了 変数や定数を削除"<<std::endl;
            }
            catch(...)
            {
                std::cout<<"BUG!!!!!!!!!!!!!!!!!!"<<std::endl;
                std::cout<<"異常終了 変数や定数を削除"<<std::endl;
            }
#endif
            running = false;
            if(endpause)std::getchar();
            clock_t start,end;
            start = clock();
            std::vector<lang::scope*> del;
            for(auto i : lang::gc->roots)
            {
                del.push_back(i.first);
            }
            lang::gc->roots.clear();
            //lang::gc->root->variable._variable.clear();
            lang::gc->start();

            end = clock();
            std::cout<<(double)(end-start)/CLOCKS_PER_SEC<<"秒"<<std::endl;
            for(int i=0;i<del.size();i++)
            {
                delete del[i];
            }
            std::cout<<"定数の削除"<<std::endl;
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
theend:
#if _DEBUG
    BreakPoint.~vector();
#endif
    delete BuiltFunction;
    delete lang::NULLOBJECT;
    delete lang::ObjectType;
    delete lang::object_tostr;
    delete lang::string_substr;
    if(leakcheck) 
    {
        _CrtDumpMemoryLeaks();
    }
    if(pause)
#if defined(_WIN32)
#if _DEBUG
        std::getchar();
#endif
#if !defined(_DEBUG)
    system("PAUSE");//WINDOWSならこっちの方が親切だから使う
#endif
#else
        std::getchar();
#endif
    return result;
}
