// lang.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//
#pragma once
#pragma 
#pragma execution_character_set("utf-8")
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
//#include "†.h"
#pragma comment(linker, "\"/manifestdependency:type='Win32' "\
	"name='Microsoft.Windows.Common-Controls' version='6.0.0.0' "\
	"processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#ifndef _DEBUG
#ifdef _WIN32
#include <eh.h>
//構造化例外が発生すると、この関数が呼ばれる
void se_translator_function(unsigned int code, struct _EXCEPTION_POINTERS* ep)
{
	throw ep; //標準C++の例外を発生させる
}
//copy http://www.ne.jp/asahi/hishidama/home/tech/vcpp/seh.html
LONG dump_exception(_EXCEPTION_POINTERS *ep, char*& a)
{
	PEXCEPTION_RECORD rec = ep->ExceptionRecord;

	TCHAR buf[1024];
	wsprintf(buf, TEXT("code:%x flag:%x addr:%p params:%d\n"),
		rec->ExceptionCode,
		rec->ExceptionFlags,
		rec->ExceptionAddress,
		rec->NumberParameters
		);
	/*::OutputDebugString*/std::wcout << (buf);

	for (DWORD i = 0; i < rec->NumberParameters; i++){
		wsprintf(buf, TEXT("param[%d]:%x\n"),
			i,
			rec->ExceptionInformation[i]
			);
		/*::OutputDebugString*/std::wcout << (buf);
	}
	a = "?";
	switch (rec->ExceptionCode)
	{
		case STATUS_WAIT_0:a = "STATUS_WAIT_0"; break;
		case STATUS_ABANDONED_WAIT_0:a = "STATUS_ABANDONED_WAIT_0"; break;
		case STATUS_USER_APC:a = "STATUS_USER_APC"; break;
		case STATUS_TIMEOUT:a = "STATUS_TIMEOUT"; break;
		case STATUS_PENDING:a = "STATUS_PENDING"; break;
		case DBG_EXCEPTION_HANDLED:a = "DBG_EXCEPTION_HANDLED"; break;
		case DBG_CONTINUE:a = "DBG_CONTINUE"; break;
		case STATUS_SEGMENT_NOTIFICATION:a = "STATUS_SEGMENT_NOTIFICATION"; break;
		case STATUS_FATAL_APP_EXIT:a = "STATUS_FATAL_APP_EXIT"; break;
		case DBG_TERMINATE_THREAD:a = "DBG_TERMINATE_THREAD"; break;
		case DBG_TERMINATE_PROCESS:a = "DBG_TERMINATE_PROCESS"; break;
		case DBG_CONTROL_C:a = "DBG_CONTROL_C"; break;
		case DBG_PRINTEXCEPTION_C:a = "DBG_PRINTEXCEPTION_C"; break;
		case DBG_RIPEXCEPTION:a = "DBG_RIPEXCEPTION"; break;
		case DBG_CONTROL_BREAK:a = "DBG_CONTROL_BREAK"; break;
		case DBG_COMMAND_EXCEPTION:a = "DBG_COMMAND_EXCEPTION"; break;
		case STATUS_GUARD_PAGE_VIOLATION:a = "STATUS_GUARD_PAGE_VIOLATION"; break;
		case STATUS_DATATYPE_MISALIGNMENT:a = "STATUS_DATATYPE_MISALIGNMENT"; break;
		case STATUS_BREAKPOINT:a = "STATUS_BREAKPOINT"; break;
		case STATUS_SINGLE_STEP:a = "STATUS_SINGLE_STEP"; break;
		case STATUS_LONGJUMP:a = "STATUS_LONGJUMP"; break;
		case STATUS_UNWIND_CONSOLIDATE:a = "STATUS_UNWIND_CONSOLIDATE"; break;
		case DBG_EXCEPTION_NOT_HANDLED:a = "DBG_EXCEPTION_NOT_HANDLED"; break;
		case STATUS_ACCESS_VIOLATION:a = "STATUS_ACCESS_VIOLATION"; break;
		case STATUS_IN_PAGE_ERROR:a = "STATUS_IN_PAGE_ERROR"; break;
		case STATUS_INVALID_HANDLE:a = "STATUS_INVALID_HANDLE"; break;
		case STATUS_INVALID_PARAMETER:a = "STATUS_INVALID_PARAMETER"; break;
		case STATUS_NO_MEMORY:a = "STATUS_NO_MEMORY"; break;
		case STATUS_ILLEGAL_INSTRUCTION:a = "STATUS_ILLEGAL_INSTRUCTION"; break;
		case STATUS_NONCONTINUABLE_EXCEPTION:a = "STATUS_NONCONTINUABLE_EXCEPTION"; break;
		case STATUS_INVALID_DISPOSITION:a = "STATUS_INVALID_DISPOSITION"; break;
		case STATUS_ARRAY_BOUNDS_EXCEEDED:a = "STATUS_ARRAY_BOUNDS_EXCEEDED"; break;
		case STATUS_FLOAT_DENORMAL_OPERAND:a = "STATUS_FLOAT_DENORMAL_OPERAND"; break;
		case STATUS_FLOAT_DIVIDE_BY_ZERO:a = "STATUS_FLOAT_DIVIDE_BY_ZERO"; break;
		case STATUS_FLOAT_INEXACT_RESULT:a = "STATUS_FLOAT_INEXACT_RESULT"; break;
		case STATUS_FLOAT_INVALID_OPERATION:a = "STATUS_FLOAT_INVALID_OPERATION"; break;
		case STATUS_FLOAT_OVERFLOW:a = "STATUS_FLOAT_OVERFLOW"; break;
		case STATUS_FLOAT_STACK_CHECK:a = "STATUS_FLOAT_STACK_CHECK"; break;
		case STATUS_FLOAT_UNDERFLOW:a = "STATUS_FLOAT_UNDERFLOW"; break;
		case STATUS_INTEGER_DIVIDE_BY_ZERO:a = "STATUS_INTEGER_DIVIDE_BY_ZERO"; break;
		case STATUS_INTEGER_OVERFLOW:a = "STATUS_INTEGER_OVERFLOW"; break;
		case STATUS_PRIVILEGED_INSTRUCTION:a = "STATUS_PRIVILEGED_INSTRUCTION"; break;
		case STATUS_STACK_OVERFLOW:a = "STATUS_STACK_OVERFLOW"; break;
		case STATUS_DLL_NOT_FOUND:a = "STATUS_DLL_NOT_FOUND"; break;
		case STATUS_ORDINAL_NOT_FOUND:a = "STATUS_ORDINAL_NOT_FOUND"; break;
		case STATUS_ENTRYPOINT_NOT_FOUND:a = "STATUS_ENTRYPOINT_NOT_FOUND"; break;
		case STATUS_CONTROL_C_EXIT:a = "STATUS_CONTROL_C_EXIT"; break;
		case STATUS_DLL_INIT_FAILED:a = "STATUS_DLL_INIT_FAILED"; break;
		case STATUS_FLOAT_MULTIPLE_FAULTS:a = "STATUS_FLOAT_MULTIPLE_FAULTS"; break;
		case STATUS_FLOAT_MULTIPLE_TRAPS:a = "STATUS_FLOAT_MULTIPLE_TRAPS"; break;
		case STATUS_REG_NAT_CONSUMPTION:a = "STATUS_REG_NAT_CONSUMPTION"; break;
		case STATUS_STACK_BUFFER_OVERRUN:a = "STATUS_STACK_BUFFER_OVERRUN"; break;
		case STATUS_INVALID_CRUNTIME_PARAMETER:a = "STATUS_INVALID_CRUNTIME_PARAMETER"; break;
		case STATUS_ASSERTION_FAILURE:a = "STATUS_ASSERTION_FAILURE"; break;
		case STATUS_SXS_EARLY_DEACTIVATION:a = "STATUS_SXS_EARLY_DEACTIVATION((DWORD)0xC015000FL)"; break;
		case STATUS_SXS_INVALID_DEACTIVATION:a = "STATUS_SXS_INVALID_DEACTIVATION"; break;
	}
	return EXCEPTION_EXECUTE_HANDLER;
}
#endif
#endif
#define END if(lang::prompt)goto theend
#define FAILEND if(lang::prompt){result = 1;goto theend;}
//#include "parseObj.h"
//#include "parserEnum.h"
namespace lang
{
	namespace plugin
	{

	}
	bool running, prompt = false;
	namespace lib{ void init(); }
	Type* ObjectTypeClass = new Type(PreType::_Object);
	langObject NULLOBJECT = nullptr;//ewObject(nullptr);
	langObject TRUEOBJECT = nullptr;
	langObject FALSEOBJECT = nullptr;
	bool ahogc = false, parserresult = false, leakcheck = false, pause = false;
#if _DEBUG
	std::vector<int> BreakPoint;//行で指定
#endif
	typedef langObject(*BuiltFunc)(std::vector<langObject>);
	extern std::map<std::string, BuiltFunc>* BuiltFunction;
}
using namespace lang;
#pragma once
char* enumtable[] = { "identifier", "num", "doublequote", "str", "leftparent", "rightparent", "comma", "plus", "minus", "multiply", "equal", "equalequal", "semicolon", "blockstart", "blockend", "plusplus", "minusminus", "greater", "less", "greaterequal", "lessequal", "modulo", "plusequal", "minusequal", "dot", "division", "leftbracket", "rightbracket", "debbug_stop", "multiplyequal", "divisionequal", "moduloequal", "or", "oror", "and", "andand", "xor", "notequal", "not", "notnot", "leftshift", "rightshift", "leftshiftequal", "rightshiftequal", "andequal", "orequal", "xorequal", "chr", "none",
"_class",
"_new",
"_this",
"_namespace",
"_using",
"_static",
"_true",
"_false",
"_null",
"_break",
"_continue",
"_else",
"_while",
"_for",
"_if",
"_return",
"base",
"colon",
"_foreach",
"_in",
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
	for (size_t i = 0; i < size; i++)
	{
		result.append(escapetable[(unsigned char)str.at(i)]);
	}
	return result;
}
bool lang::gc_view = false;
extern int lang::error_level;
__v('ω')__v('ω')__v('ω')
//#define out auto
//v（’ω’）v
//    v（’ω’）v
//    v（’ω’）v
std::string getlinestring(std::string input, int index)
{
	int i, j;
	for (i = index; i >= 0; i--)
	{
		if (input[i] == '\n') break;
	}
	for (j = index; j < input.size(); j++)
	{
		if (input[j] == '\n') break;
	}
	return input.substr(i + 1, j - i - 1);
}
enum ENUMCLASS option
{
	none, errorlevel, std_out, chdir, std_err
};
int hook(int a1, char *a2, int *a3)
{
	std::cout << a2;
	return a1;
}
//面倒になったからコピペhttp://d.hatena.ne.jp/kryozahiro/20080809/1218295912
//->wcstombs_s使う
void narrow(const std::wstring &src, std::string &dest) {
	char *mbs = new char[src.length() * MB_CUR_MAX + 1];
	wcstombs_s(NULL, mbs, src.length() * MB_CUR_MAX + 1, src.c_str(), src.length() * MB_CUR_MAX + 1);
	dest = mbs;
	delete[] mbs;
}
#include "GTKOLWindow.h"
OLWindow* window;
char filename_[MAX_PATH];
#define _OLT(x) x
char*filename = _OLT("");
TextBox* txt;
bool UTF8 = false;
#ifdef OL_GTK
#define SETUTF8 UTF8 = true
#else
#define SETUTF8 
#endif
//見にくいから分けた
void Button_OnClick(eventargs<OLWindow*> c)
{
	OpenFileDialog ofd(_OLT("OL(*.OL)\0*.OL\0All files(*.*)\0*.*\0\0"), _OLT("OL"), _OLT("OpenFileDialog"), *window);
	ofd.Show();
	strcpy_s((char*)filename_, sizeof(filename_), ofd.GetFileName());
	//lstrcpynW(filename_,ofd.GetFileName(),sizeof(filename_));
	filename = filename_;
	SETUTF8;
	txt->SetText(ofd.GetFileName());
}
void Button2_OnClick(eventargs<OLWindow*> c)
{
	txt->GetText(filename_, sizeof(filename_));
	window->Close();
	SETUTF8;
}
#ifdef qOL_GTK
void Window_OnSizeChange(eventargs<OLWindow*> arg)
{
	std::cout<<arg.Arg->GetWidth()<<"\t"<<arg.Arg->GetHeight()<<std::endl;
	txt->SetHeight(arg.Arg->GetHeight());
	txt->SetWidth(arg.Arg->GetWidth());
}
void gui(void)
{

	window = new OLWindow(L"OtyaLanguage",512,128);
	window->SetResize(FALSE);
	Button btn(*window,L"File",512-128,0,128,32);
	Button btn_2(*window,L"Run",512-128,32,128,32);
	Label label(*window,L"OtyaLanguage",8,32,128,128);
	txt = new TextBox(*window,L"",0,0,512-128,32,false);
	btn.OnClick+=Button_OnClick/*[](eventargs<OLWindow*> c)
							   {
							   OpenFileDialog ofd(_T("OL(*.OL)\0*.OL\0All files(*.*)\0*.*\0\0"),_T("OL"),_T("OpenFileDialog"),*window);
							   ofd.Show();
							   lstrcpynW(filename_,ofd.GetFileName(),sizeof(filename_));
							   filename = filename_;
							   txt->SetText(ofd.GetFileName());
							   }*/;
	btn_2.OnClick+=Button2_OnClick/*[](eventargs<OLWindow*> c)
								  {
								  txt->GetText(filename_,sizeof(filename_));
								  window->Close();
								  }*/;
	window->Show();
	delete window;
	// delete txt;
	return;
#endif
#ifndef __v('ω')
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
#else
	void ehatest(void)
	{
		/*__try
		{
		*((int*)10) = 0;
		}
		__except (dump_exception(GetExceptionInformation()))
		{
		}*/
	}
	void gui(void)
	{
		//ehatest();
		window = new OLWindow("OtyaLanguage", 512, 128);
		window->SetResize(FALSE);
		Button btn(*window, "File", 512 - 128, 0, 128, 32);
		Button btn_2(*window, "Run", 512 - 128, 32, 128, 32);
		txt = new TextBox(*window, _OLT(""), 0, 0, 512 - 128, 32, false);
		Label label(*window, _OLT("OtyaLanguage"), 8, 32, 128, 128);
		label.SetFont("Meiryo UI", 20);
		btn.OnClick += Button_OnClick;
		btn_2.OnClick += Button2_OnClick;
		window->Show();
		delete window;
		delete txt;
#endif
	}
#include <commctrl.h>
#ifndef _WIN32
#include <unistd.h>
#endif
#pragma comment(lib, "comctl32.lib")
	int main(int argc, char *argv[])
	{
		INITCOMMONCONTROLSEX    stICCEx;
		stICCEx.dwSize = sizeof (INITCOMMONCONTROLSEX);
		stICCEx.dwICC = INT_MAX;//ICC_WIN95_CLASSES;                                // ICC_STANDARD_CLASSES
		//InitCommonControlsEx
		InitCommonControlsEx(&stICCEx);
		//    _CrtSetBreakAlloc(155);
		int result = 0;
		{
			wchar_t et[256];
			for (unsigned char i = 0; i < 255; i++)
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
			//スレッド毎に変換関数を登録する
			_set_se_translator(se_translator_function);
#endif
			//_CrtSetReportMode( 1, _CRTDBG_MODE_WNDW );
			//_CrtSetBreakAlloc(223);_CrtSetBreakAlloc(221);
			//_CrtSetBreakAlloc(218);
			option o = option::none;
			lang::error_level = 0;
			bool endpause = false, json = false, nologo = false;
			bool notfileload = true;
			bool gui = !!!!false;
			std::string input;
			std::vector<char*> files;
#pragma region command line
			for (int i = 1; i < argc; i++)
			{
				switch (o)
				{
					case option::none:
						if (!/*_tcscmp*/strcmp(argv[i], _OLT("-ahogc")))
						{
							ahogc = true;
						}
						else
						if (!strcmp(argv[i], _OLT("-parserresult")))
						{
							parserresult = true;
						}
						else
						if (!strcmp(argv[i], _OLT("-gcview")))
						{
							lang::gc_view = true;
						}
						else
						if (!strcmp(argv[i], _OLT("-leakcheck")))
						{
							leakcheck = true;
						}
						else
						if (!strcmp(argv[i], _OLT("-pause")))
						{
							pause = true;
						}
						else
						if (!strcmp(argv[i], _OLT("-endpause")))
						{
							endpause = true;
						}
						else
						if (!strcmp(argv[i], _OLT("-json")))
						{
							json = true;
						}
						else
						if (!strcmp(argv[i], _OLT("-errorlevel")))
						{
							o = option::errorlevel;
						}
						else
						if (!strcmp(argv[i], _OLT("-e")))
						{
							input = argv[i + 1];//narrow(std::wstring(argv[i + 1]),input);
							notfileload = true;
						}
						else
						if (!strcmp(argv[i], _OLT("-gui")))
						{
							gui = true;
						}
						else
						if (!strcmp(argv[i], _OLT("-prompt")))
						{
							lang::prompt = true;
							notfileload = true;
						}
						else
						if (!strcmp(argv[i], _OLT("-nologo")))
						{
							nologo = true;
						}
						else
						if (!strcmp(argv[i], _OLT("-stdout")))
						{
							o = option::std_out;
						}
						else
						if (!strcmp(argv[i], _OLT("-stderr")))
						{
							o = option::std_err;
						}
						else
						if (!strcmp(argv[i], _OLT("-chdir")))
						{
							o = option::chdir;
						}
						else
						{
							//filename = argv[i];
							notfileload = false;
							files.push_back(argv[i]);
						}
						break;
					case option::errorlevel:
						lang::error_level = /*_ttoi*/atoi(argv[i]);
						o = option::none;
						break;
					case option::std_out:
					{
											auto s = stdout;
											freopen_s(&s, argv[i], "w+", stdout);
											/*
											auto s = stdout;
											fopen_s(&s, argv[i], "w+");*/
											o = option::none;
					}
						break;
					case option::std_err:
					{
											auto s = stderr;
											freopen_s(&s, argv[i], "w+", stderr);
											o = option::none;
					}
						break;
					case option::chdir:
#ifndef _WIN32
						chdir(argv[i]);
#endif
#ifdef _WIN32
						SetCurrentDirectoryA(argv[i]);
#endif
						o = option::none;
						break;
					default:
						break;
				}
			}
			if (input.size() == 0 && notfileload)
			{
				prompt = true;
			}
#pragma endregion
			if (gui)
			{
				::gui();
			}
			if (!json && !nologo)
#if _DEBUG
				std::cout << "OtyaLanguage DEBUG build" << std::endl;
#else
				std::cout << "OtyaLanguage" << std::endl;
#endif
			if (prompt)
			{
				std::cout << "Prompt Mode" <<
					std::endl <<
					"Help: #Help" <<
					std::endl;
			}
		http://0xbaadf00d/
			//lang::gc_view = true;
			if (leakcheck) _CrtSetReportHook((_CRT_REPORT_HOOK)hook);
			lib::init();
			lang::NULLOBJECT = new lang::Object();
			lang::NULLOBJECT->type->name = "null";
			lang::TRUEOBJECT = new lang::Int(true);
			lang::TRUEOBJECT->type->name = "true";
			lang::FALSEOBJECT = new lang::Int(false);
			lang::FALSEOBJECT->type->name = "false";
			do//while (true)//std::getchar())
			{
				std::stringstream ss;
				std::ifstream ifs;
				//std::getline(std::cin,input);
				if (!notfileload)
				{
					for (int i = 0; i < files.size(); i++)
					{
						filename = files[i];
#if _WIN32
						if (UTF8)
						{
							int len = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)filename, -1, NULL, 0);
							char* utf16 = new char[len * 2 + 2];
							MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)filename, -1, (LPWSTR)utf16, len);
							int sjislen = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)utf16, -1, NULL, 0, NULL, NULL);
							char* buffSJis = new char[sjislen * 2];
							ZeroMemory(buffSJis, sjislen * 2);
							WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)utf16, -1, (LPSTR)buffSJis, sjislen, NULL, NULL);
							delete utf16;
							ifs = std::ifstream(buffSJis);
							delete buffSJis;
						}
						else
						{
							ifs = std::ifstream(filename);
						}
#else
						ifs = std::ifstream(filename);
#endif
						if (ifs.fail())
						{
							/*_tc*/printf(_OLT("file:%sを開けません\n"), filename);//std::cout<<"file:"<<filename<<"を開けません"<<std::endl;
							__v('ω')v____v('ω')FAILEND; __v('ω')v____v('ω')
						}
						while (ifs && getline(ifs, input)) {
							ss << input << '\n';
						}
						input = ss.str();
					}
				}
				else
				if (lang::prompt)
				{
					std::getline(std::cin, input);
				}
				lang::parser* pars = 0;
				try
				{
					pars = new lang::parser(input);//アウト
					if (lang::prompt)
					{
						if (pars->parsers.size() > 1)
						{
							if (pars->parsers[0]->pEnum == parserEnum::none && *pars->parsers[0]->name == "#")
							{
								if (pars->parsers[1]->pEnum == parserEnum::identifier && *pars->parsers[1]->name == "help")
								{
									std::cout << "Help" << std::endl <<
										"#cmd [...] #cmd help" << std::endl;
									continue;
								}
								if (pars->parsers[1]->pEnum == parserEnum::identifier && *pars->parsers[1]->name == "cmd")
								{
									system(input.substr(pars->parsers[1]->sourceendindex + 1).c_str());
									continue;
								}
							}
						}
					}
				}
				catch (lang::langParseException ex)
				{
					std::cerr << std::endl << "lang::langParseException - lang::parser" << std::endl << ex.what();
					continue;
				}
				catch (lang::langRuntimeException ex)
				{
					std::cerr << std::endl << "lang::langRuntimeException - lang::scope::run" << std::endl << ex.what() << std::endl << "場所?:" << std::endl;
#if CPP11
					for (auto i : ex.stacktrace)
					{
#else
					for (auto it = ex.stacktrace.begin(); it != ex.stacktrace.end(); ++it)
					{
						auto i = *it;
#endif
						std::cerr << getlinestring(input, ex.tokens[i.second]->sourcestartindex);
						//std::cout << input.substr(ex.tokens[i.first]->sourcestartindex, ex.tokens[i.second]->sourceendindex - ex.tokens[i.first]->sourcestartindex + 1) << std::endl;
						//break;
					}
					std::cerr << "StackTrace" << std::endl;
					FOREACH(i, ex.funcstacktrace)//for(auto i : ex.funcstacktrace)
						//{
						std::cerr << i << std::endl;
					ENDFOREACH
					if (lang::gc_view)std::cerr << "異常終了 変数や定数を削除" << std::endl;
					//std::getchar();
					__v('ω')__v('ω')FAILEND; __v('ω')__v('ω') //continue;
				}
				for (int i = 0; i < pars->parsers.size(); i++)
				{
					if (pars->parsers[i]->ptr != nullptr)
					{
						lang::gc->constroot.push_back(pars->parsers[i]->ptr);
					}
				}
				//out testobj=new lang::parseObj("hoge");//アウト
				//std::cout<<pars->program<<std::endl<<testobj->getString()<<std::endl;*pars->parsers[i]->toString()*/
				int nest = 0;
				if (json)
				{
					std::cout << "{\"parser\":[";
					//                for(auto &&i : pars->parsers)
					FOREACH(i, pars->parsers)//                {
						std::cout << '{' << "\"name\":\"" << (i->name ? escape(*i->name) : "") << '\"' << ',' << "\"pEnum\":\"" << enumtable[i->pEnum] << '\"' << ',' << "\"ptr\":";
					if (i->ptr)
					{
						if (i->ptr is _Int)
						{
							std::cout << *(int*)i->ptr->getPointer();
						}
						else
						if (i->ptr is _String)
						{
							std::cout << '\"' << escape(*(std::string*)i->ptr->getPointer()) << '\"';
						}
					}
					else
					{
						std::cout << "null";
					}
					std::cout << '}' << ',';
					ENDFOREACH
						std::cout << ']' << ',';
					std::cout << '}';
					exit(0);
				}
				if (parserresult)
				for (int i = 0; i < pars->parsers.size(); i++)
				{
					if (pars->parsers[i]->pEnum == lang::blockend)nest--;
					if (pars->parsers[i]->pEnum == lang::rightparent)nest--;
					std::cout << i << "\t";
					for (int j = 0; j < nest; j++)std::cout << " ";
					std::cout << input.substr(pars->parsers[i]->sourcestartindex, pars->parsers[i]->sourceendindex - pars->parsers[i]->sourcestartindex + 1) << "\t" << parserEnumToString(pars->parsers[i]->pEnum) << std::endl;
					if (pars->parsers[i]->pEnum == lang::blockstart)nest++;
					if (pars->parsers[i]->pEnum == lang::leftparent)nest++;
				}
				try
				{
					__v('ω')v__
						__v('ω')v__
						__v('ω')v__
						//#if AHO_GC //スレッドで動き続けるアホなGC
#ifdef CPP11
					if (ahogc)std::thread thd([]{ while (true) lang::gc->start(); });
#endif
					//#endif
					// lang::NULLOBJECT = new lang::Object();
					running = true;
					pars->runner->refinc();
					auto result = pars->Run();//pars->runner->run();
					if (result && result != NULLOBJECT) std::cout << result->toString();// << std::endl;
					if (lang::gc_view)std::cout << "実行終 変数や定数を削除" << std::endl;
				}
				catch (lang::langRuntimeException ex)
				{
					std::cerr << std::endl << "lang::langRuntimeException - lang::scope::run" << std::endl << ex.what() << std::endl << "場所?:" << std::endl;
					//                    for(auto i : ex.stacktrace)
					FOREACH(i, ex.stacktrace)//                    {
						std::cerr << getlinestring(input, ex.tokens[i.second]->sourcestartindex);
					//std::cout << input.substr(ex.tokens[i.first]->sourcestartindex, ex.tokens[i.second]->sourceendindex - ex.tokens[i.first]->sourcestartindex + 1) << std::endl;
					//break;
				}
				std::cerr << "StackTrace" << std::endl;
				FOREACH(i, ex.funcstacktrace)//                    for(auto i : ex.funcstacktrace)
					//{
					std::cerr << i << std::endl;
				ENDFOREACH//}
				if (lang::gc_view)std::cerr << "異常終了 変数や定数を削除" << std::endl;
			}
#if !defined(_DEBUG)//デバッグならデバッガの機能を利用する
			catch (std::exception ex)
			{
				std::cerr << "BUG!!!" << ex.what() << std::endl;
				std::cerr << "異常終了 変数や定数を削除" << std::endl;
			}
			catch (struct _EXCEPTION_POINTERS* ep)
			{
				std::cerr << "WIN32 EXCEPTION!" << std::endl;
				char* codestring;
				dump_exception(ep, codestring);
				std::cerr << codestring << std::endl;
				if (lang::gc_view)std::cerr << "異常終了 変数や定数を削除" << std::endl;
			}
			catch (...)
			{
				std::cerr << "BUG!!!!!!!!!!!!!!!!!!" << std::endl;
				if (lang::gc_view)std::cerr << "異常終了 変数や定数を削除" << std::endl;
			}
#endif
			running = false;
			if (endpause)std::getchar();
			clock_t start, end;
			start = clock();
			std::vector<lang::scope*> del;
			//                for(auto i : lang::gc->roots)
			FOREACH(i, lang::gc->roots)//                {
				del.push_back(i.first);
			ENDFOREACH
				lang::gc->roots.clear();
			//lang::gc->root->variable._variable.clear();
			lang::gc->start();

			end = clock();
			if (lang::gc_view)std::cout << (double)(end - start) / CLOCKS_PER_SEC << "秒" << std::endl;
			for (int i = 0; i < del.size(); i++)
			{
				delete del[i];
			}
			if (lang::gc_view)std::cout << "定数の削除" << std::endl;
			for (int i = 0; i < pars->parsers.size(); i++)
			{
				// delete pars->parsers[i]->ptr;
				delete pars->parsers[i];
			}
			delete pars;
			if (prompt)std::cout << std::endl;
			//(new lang::scope(pars->parsers))->run();
			//}
			//catch(char* ex)
			//{
			//    std::cout<< ex<<std::endl;3333
			//}
			//delete pars;
			//break;
		}
		while (prompt);
	theend:
#if _DEBUG
		BreakPoint.~vector();
#endif
		delete BuiltFunction;
		delete lang::NULLOBJECT;
		delete lang::TRUEOBJECT;
		delete lang::FALSEOBJECT;
		delete lang::ObjectTypeClass;
		delete lang::object_tostr;
		delete lang::string_substr;
	}
	if (leakcheck)
	{
		_CrtDumpMemoryLeaks();
	}
	if (pause)
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
