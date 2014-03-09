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
#ifdef CPP11
#include <thread>
#endif
#include <windows.h>
#include "Function.h"
#include <time.h>
#include "OLWindow.h"
#include "��.h"
#pragma comment(linker, "\"/manifestdependency:type='Win32' "\
    "name='Microsoft.Windows.Common-Controls' version='6.0.0.0' "\
    "processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#ifndef _DEBUG
#include <eh.h>
//�\������O����������ƁA���̊֐����Ă΂��
void se_translator_function(unsigned int code, struct _EXCEPTION_POINTERS* ep)
{
    throw ep; //�W��C++�̗�O�𔭐�������
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
    langObject TRUEOBJECT = nullptr;
    langObject FALSEOBJECT = nullptr;
    bool ahogc = false,parserresult = false, leakcheck = false, pause = false;
#if _DEBUG
    std::vector<int> BreakPoint;//�s�Ŏw��
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
    "_true",
    "_false",
    "_null",
};

char* parserEnumToString(lang::parserEnum i)
{
    return enumtable[i];
}
char* escapetable[256];
std::string escape(std::string& str)
{
    std::string result;
    size_t size = str.length();
    for(size_t i=0;i<size;i++)
    {
        result.append(escapetable[(unsigned char)str.at(i)]);
    }
    return result;
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
//�ʓ|�ɂȂ�������R�s�yhttp://d.hatena.ne.jp/kryozahiro/20080809/1218295912
//->wcstombs_s�g��
void narrow(const std::wstring &src, std::string &dest) {
    char *mbs = new char[src.length() * MB_CUR_MAX + 1];
    wcstombs_s(NULL,mbs, src.length() * MB_CUR_MAX + 1, src.c_str(), src.length() * MB_CUR_MAX + 1);
    dest = mbs;
    delete [] mbs;
}
TextBox* txt;
OLWindow* window;
void Window_OnSizeChange(eventargs<OLWindow*> arg)
{
    std::cout<<arg.Arg->GetWidth()<<"\t"<<arg.Arg->GetHeight()<<std::endl;
    txt->SetHeight(arg.Arg->GetHeight());
    txt->SetWidth(arg.Arg->GetWidth());
}
TCHAR filename_[MAX_PATH];
_TCHAR*filename = L"";
//���ɂ������番����
void gui(void)
{

    window = new OLWindow(L"OtyaLanguage",512,128);
    window->SetResize(FALSE);
    Button btn(*window,L"File",512-128,0,128,32);
    Button btn_2(*window,L"Run",512-128,32,128,32);
    Label label(*window,L"OtyaLanguage",8,32,128,128);
    txt = new TextBox(*window,L"",0,0,512-128,32,false);
    btn.OnClick+=[](eventargs<OLWindow*> c)
    {
        OpenFileDialog ofd(_T("OL(*.OL)\0*.OL\0All files(*.*)\0*.*\0\0"),_T("OL"),_T("OpenFileDialog"),*window);
        ofd.Show();
        lstrcpynW(filename_,ofd.GetFileName(),sizeof(filename_));
        filename = filename_;
        txt->SetText(ofd.GetFileName());
    };
    btn_2.OnClick+=[](eventargs<OLWindow*> c)
    {
        txt->GetText(filename_,sizeof(filename_));
        window->Close();
    };
    window->Show();
    delete window;
    // delete txt;
    return;
#ifndef v�i�f�ցf�jv
    //DEBUG
    window = new OLWindow(L"Hello",512,512);
    /*Button*/ btn = Button(*window,L"Button",0,0,128,32);
    Button btn2(*window,L"Button2",128,0,128,32);
    Button btn3(*window,L"Button3",0,32,128,32);
    txt = new TextBox(*window,L"Text",0,64,512,512-64);
    txt->SetMultiLine(TRUE);
    Button btn4(*txt,L"Button4",0,32,128,32);
    btn.OnClick+=[](eventargs<OLWindow*> c){std::cout<<"Button1"<<txt->GetReadOnly();txt->SetReadOnly(!txt->GetReadOnly());};
    btn.OnClick+=[](eventargs<OLWindow*> c){std::cout<<std::endl;};
    btn2.OnClick+=[](eventargs<OLWindow*> c){std::cout<<"Button2"<<txt->GetReadOnly();txt->SetReadOnly(!txt->GetReadOnly());};
    btn3.OnClick+=[](eventargs<OLWindow*> c)
    {
        std::cout<<"Button3";
        OpenFileDialog ofd(_T("OL(*.OL)\0*.txt\0All files(*.*)\0*.*\0\0"),_T("OL"),_T("OpenFileDialog"),*window);
        ofd.Show();
        lstrcpynW(filename_,ofd.GetFileName(),sizeof(filename_));
        filename = filename_;
    };
    window->OnSizeChange+=Window_OnSizeChange;
    window->Show();
    delete window;
    delete txt;
#endif
}
int _tmain(int argc, _TCHAR *argv[])
{
    int result = 0;
    {
        wchar_t et[256];
        for(unsigned char i=0;i<255;i++)
        {
            wchar_t a;
            ((char*)&a)[0] = i;
            ((char*)&a)[1] = '\0';
            et[(unsigned char)i] = i;
            escapetable[(unsigned char)i] = (char*)&(et[(unsigned char)i]);
        }
        escapetable[10] = "\\n";
        escapetable[13] = "\\r";
        escapetable[(int)'\"'] = "\\\"";
        //([](int x){return x*x;})();
#ifndef _DEBUG
        //�X���b�h���ɕϊ��֐���o�^����
        _set_se_translator(se_translator_function);
#endif
        //_CrtSetReportMode( 1, _CRTDBG_MODE_WNDW );
        _CrtSetBreakAlloc(223);_CrtSetBreakAlloc(221);
        _CrtSetBreakAlloc(218);
        option o = option::none;
        lang::error_level = 0;
        bool endpause= false,json = false;
        bool notfileload = false;
        bool gui = false;
        std::string input;
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
                                                if(!_tcscmp(argv[i], L"-e"))
                                                {
                                                    narrow(std::wstring(argv[i + 1]),input);
                                                    notfileload = true;
                                                }
                                                else
                                                    if(!_tcscmp(argv[i], L"-gui"))
                                                    {
                                                        gui = true;
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
        if(gui)
        {
            ::gui();
        }
        if(!json)
#if _DEBUG
            std::cout<<"OtyaLanguage DEBUG build"<<std::endl;
#else
            std::cout<<"OtyaLanguage"<<std::endl;
#endif
http://0xbaadf00d/
        //lang::gc_view = true;
        if(leakcheck) _CrtSetReportHook((_CRT_REPORT_HOOK)hook);
        lib::init();
        lang::NULLOBJECT = new lang::Object();
        lang::NULLOBJECT->type->name = "null";
        lang::TRUEOBJECT = new lang::Int(true);
        lang::TRUEOBJECT->type->name = "true";
        lang::FALSEOBJECT = new lang::Int(false);
        lang::FALSEOBJECT->type->name = "false";
        while (true)//std::getchar())
        {
            std::stringstream ss;
            //std::getline(std::cin,input);
            if(!notfileload)
            {
                std::ifstream ifs( filename);
                if(ifs.fail())
                {
                    _tcprintf(L"file:%s���J���܂���\n",filename);//std::cout<<"file:"<<filename<<"���J���܂���"<<std::endl;
                    v�i�f�ցf�jv ;v�i�f�ցf�jv ;FAILEND;v�i�f�ցf�jv v�i�f�ցf�jv 
                }
                while(ifs && getline(ifs, input)) {
                    ss << input << '\n';
                }
                input = ss.str();
            }
            lang::parser* pars;
            try
            {
                pars = new lang::parser(input);//�A�E�g
            }
            catch(lang::langParseException ex)
            {
                std::cout<<std::endl<<"lang::langParseException - lang::parser"<<std::endl<<ex.what();
                continue;
            }
            catch(lang::langRuntimeException ex)
            {
                std::cout << std::endl << "lang::langRuntimeException - lang::scope::run" << std::endl << ex.what() << std::endl << "�ꏊ?:" << std::endl;
                for(auto i : ex.stacktrace)
                {
                    std::cout << �Ă��k�h�m�d����Ă�H(input,ex.tokens[i.second]->sourcestartindex);
                    //std::cout << input.substr(ex.tokens[i.first]->sourcestartindex, ex.tokens[i.second]->sourceendindex - ex.tokens[i.first]->sourcestartindex + 1) << std::endl;
                    //break;
                }
                std::cout << "StackTrace" << std::endl;
                for(auto i : ex.funcstacktrace)
                {
                    std::cout << i << std::endl;
                }
                std::cout<<"�ُ�I�� �ϐ���萔���폜"<<std::endl;
                std::getchar();
                v�i�f�ցf�jv ;v�i�f�ցf�jv ;FAILEND;v�i�f�ցf�jv v�i�f�ցf�jv //continue;
            }
            for(int i=0;i<pars->parsers.size();i++)
            {
                if(pars->parsers[i]->ptr != nullptr)
                {
                    lang::gc->constroot.push_back(pars->parsers[i]->ptr);
                }
            }
            //out testobj=new lang::parseObj("hoge");//�A�E�g
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
                    v�i�f�ցf�jv
                        v�i�f�ցf�jv
                        v�i�f�ցf�jv
                        //#if AHO_GC //�X���b�h�œ���������A�z��GC
#ifdef CPP11
                        if(ahogc)std::thread thd([]{ while (true) lang::gc->start();});
#endif
                    //#endif
                    // lang::NULLOBJECT = new lang::Object();
                    running = true;
                    pars->runner->run();
                    std::cout<<"���s�I �ϐ���萔���폜"<<std::endl;
                }
                catch(lang::langRuntimeException ex)
                {
                    std::cout << std::endl << "lang::langRuntimeException - lang::scope::run" << std::endl << ex.what() << std::endl << "�ꏊ?:" << std::endl;
                    for(auto i : ex.stacktrace)
                    {
                        std::cout << �Ă��k�h�m�d����Ă�H(input,ex.tokens[i.second]->sourcestartindex);
                        //std::cout << input.substr(ex.tokens[i.first]->sourcestartindex, ex.tokens[i.second]->sourceendindex - ex.tokens[i.first]->sourcestartindex + 1) << std::endl;
                        //break;
                    }
                    std::cout << "StackTrace" << std::endl;
                    for(auto i : ex.funcstacktrace)
                    {
                        std::cout << i << std::endl;
                    }
                    std::cout<<"�ُ�I�� �ϐ���萔���폜"<<std::endl;
                }
#if !defined(_DEBUG)//�f�o�b�O�Ȃ�f�o�b�K�̋@�\�𗘗p����
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
                std::cout<<(double)(end-start)/CLOCKS_PER_SEC<<"�b"<<std::endl;
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
theend:
#if _DEBUG
        BreakPoint.~vector();
#endif
        delete BuiltFunction;
        delete lang::NULLOBJECT;
        delete lang::TRUEOBJECT;
        delete lang::FALSEOBJECT;
        delete lang::ObjectType;
        delete lang::object_tostr;
        delete lang::string_substr;
    }
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
    system("PAUSE");//WINDOWS�Ȃ炱�����̕����e�؂�����g��
#endif
#else
        std::getchar();
#endif
    return result;
}
