#include "stdafx.h"
#include <cmath>
#include <stdlib.h>
#include <sstream>
#include <iosfwd>
#include "Function.h"
#include "GC.h"
#include "langException.h"
#include "lang.h"
#include "Class.h"
#include "Array.h"
#ifdef OL_GTK
#include "GTKOLWindow.h"
#else
#include "OLWindow.h"
#endif
#ifndef _WIN32
#define fopen_s(a,b,c) *a = fopen(b,c) 
#else
#include <windows.h>
#endif
//組み込み関数達
namespace lang
{//VisualStadio
	typedef langObject(*BuiltFunc)(std::vector<langObject>);
	extern std::map<std::string, BuiltFunc>* BuiltFunction;
	namespace lib
	{
#ifdef CPP11
		std::mutex printm;
#endif
		///thread safe?
		langObject print(std::vector<langObject> arg)
		{
			{
#ifdef CPP11
				std::lock_guard<std::mutex> lock(printm);//lock
#endif
				//if(locktst)while(locktst); locktst = true;
#if CPP11
				for (auto var : arg)
				{
#else
				for (auto it = arg.begin(); it != arg.end(); ++it)
				{
					auto var = *it;
#endif
					if (var != nullptr)std::cout << var->toString(); else std::cout << "nullptr";
				}
				//locktst = false;
			}
			return NULLOBJECT;
		}
		langObject getline(std::vector<langObject> arg)
		{
			std::string str;
			std::getline(std::cin, str);
			return newString(&str);
		}
		langObject GC(std::vector<langObject> arg)
		{
#if _DEBUG
			if (gc_view)std::cout << "明示的ながべこれ呼び出し" << std::endl;
#endif
			gc->start();
			return NULLOBJECT;
		}
		langObject free(std::vector<langObject> arg)
		{
#if CPP11
			foreach_(var_ i in_ arg)
			{
#else
			for (auto it = arg.begin(); it != arg.end(); ++it)
			{
				auto i = *it;
#endif
				gc->free_(i);
			}
			return NULLOBJECT;
		}
		langObject uncontrollGC(std::vector<langObject> arg)
		{
#if CPP11
			foreach_(var_ i in_ arg)
			{
#else
			for (auto it = arg.begin(); it != arg.end(); ++it)
			{
				auto i = *it;
#endif
				gc->uncontroll(i);
			}
			return NULLOBJECT;
		}
		langObject VarPtr(std::vector<langObject> arg)
		{
			auto i = arg[0]->getPointer();
			return newInt(*reinterpret_cast<int*>(&i));
		}
		langObject WriteMemory(std::vector<langObject> arg)
		{
			auto ptr = (int*)Int::toInt(arg[0]);
			*ptr = Int::toInt(arg[1]);
			return NULLOBJECT;
		}
		langObject ReadMemoryInt(std::vector<langObject> arg)
		{
			auto ptr = (int*)Int::toInt(arg[0]);
			return newInt(*ptr);
		}
		langObject sqrt(std::vector<langObject> arg)
		{
			return /*std::make_shared<Int>*/newInt((int)std::sqrt((double)lang::Int::toInt(arg[0])));
		}
#ifdef CPP11
		langObject thread(std::vector<langObject> arg)
		{
			langFunction threadFunc = Function::CopyFunction((langFunction)arg[0], ((langFunction)arg[0])->thisscope);
			std::thread* thd = new std::thread([threadFunc]
			{
				lang::stacktrace = new std::vector<langFunction>();
				std::vector<langObject> rarg;
				threadFunc->call(&rarg);
				threadFunc->thread->detach();
				lang::gc->free_(threadFunc);
			});
			threadFunc->thread = thd;
			return NULLOBJECT;
		}
		langObject thread_join(std::vector<langObject> arg)
		{
			langFunction threadFunc = Function::CopyFunction((langFunction)arg[0], ((langFunction)arg[0])->thisscope);
			bool thread_end = false;
			std::thread* thd = new std::thread([threadFunc, &thread_end]
			{
				lang::stacktrace = new std::vector<langFunction>();
				std::vector<langObject> rarg;
				threadFunc->call(&rarg);
				threadFunc->thread->detach();
				lang::gc->free_(threadFunc);
				thread_end = true;
				delete lang::stacktrace;
			});
			threadFunc->thread = thd;
			try
			{
				if (!thread_end)thd->join();
			}
			catch (...)
			{
			}
			return NULLOBJECT;
		}
		langObject threadid(std::vector<langObject> arg)
		{
			std::stringstream a;
			a << std::this_thread::get_id();
			return newString(a.str());
		}
#endif
		langObject Fopen(std::vector<langObject> arg)
		{
			FILE* fp;
			fopen_s(&fp, arg[0]->toString().c_str(), arg[1]->toString().c_str());
			return newObject((void*)fp);
		}
		langObject Fclose(std::vector<langObject> arg)
		{
			return newObject((void*)fclose((FILE*)arg[0]->getPointer()));
		}
		langObject Fwrite(std::vector<langObject> arg)
		{
			auto str = arg[0]->toString();
			return newObject((void*)fwrite(str.data(), sizeof(char), str.size(), (FILE*)arg[1]->getPointer()/*(FILE*)(size_t)Int::toInt(arg[1])*/));
		}
		langObject getstdin(std::vector<langObject> arg)
		{
			return newObject((void*)stdin);
		}
		langObject getstdout(std::vector<langObject> arg)
		{
			return newObject((void*)stdout);
		}
		langObject getstderr(std::vector<langObject> arg)
		{
			return newObject((void*)stderr);
		}
		langObject olruntime_gc_objectcount(std::vector<langObject> arg)
		{
			return newInt(lang::gc->objectCount);
		}
		langObject olruntime_gc_gctiming(std::vector<langObject> arg)
		{
			return newInt(lang::gc->GCtimig);
		}
		langObject olruntime_gc_run(std::vector<langObject> arg)
		{
			return newInt(lang::gc->NowGabekore);
		}
		template<class T>
		class ObjectWindow : public Object
		{
		public:
			ObjectWindow()
			{
				this->ptr = win;
			}
			~ObjectWindow()
			{
				delete this->win;
			}
			std::map<std::string, langFunction> Event;
			T* win;
		};
		typedef ObjectWindow<OLWindow> ObjectWin;
		typedef ObjectWindow<Button> ObjectBtn;
		typedef ObjectWindow<Label> ObjectLabel;
		typedef ObjectWindow<TextBox> ObjectTextBox;
		typedef ObjectWindow<CheckBox> ObjectCheckBox;
		langObject window_create(std::vector<langObject> arg)
		{
			ObjectWin* a = new ObjectWin();
			std::string str2 = arg[0]->toString();
			//int len = str2.length();
			//TCHAR* buf = new TCHAR[len * 2];
			//buf[MultiByteToWideChar(CP_OEMCP,MB_PRECOMPOSED,str2.c_str(),len,buf,len * 2)] = _T('\0');
			a->win = new OLWindow(str2.c_str()/*buf*/, Int::toInt(arg[1]), Int::toInt(arg[2]));
			//delete buf;
			//OLWindow::windowmap[a->win->hWnd] = &a->win;
			a->win->Tag = a;
			a->win->SetFont("MS UI Gothic", 9);
			//
			return a;
		}
		langObject window_show(std::vector<langObject> arg)
		{
			ObjectWin* a = dynamic_cast<ObjectWin*>(arg[0]);
			a->win->Show();
			return a;
		}
		langObject window_gettext(std::vector<langObject> arg)
		{
			ObjectWin* a = static_cast<ObjectWin*>(arg[0]);
			//TCHAR* text = a->win->GetText();
			langString result;
			//int len = _tcsclen(text);
			//char *mbs = new char[len * 2 + 2];
			//mbs[WideCharToMultiByte(CP_ACP,0,text,-1,mbs,len * 2 + 1,NULL,NULL)] = '\0';
			//result = newString(&std::string(mbs));
			//delete [] mbs;
			//delete [] text;
			result = newString(a->win->GetText());
			return result;
		}
		langObject window_settext(std::vector<langObject> arg)
		{
			ObjectWin* a = static_cast<ObjectWin*>(arg[0]);
			std::string str2 = arg[1]->toString();
			//int len = str2.length();
			//TCHAR* buf = new TCHAR[len * 2 + 1];
			//buf[MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,str2.c_str(),len,buf,len * 2)] = _T('\0');
			a->win->SetText(str2.c_str());
			//delete buf;
			return a;
		}/*
		 class ObjectBtn : public ObjectWin
		 {
		 public:
		 ObjectBtn()
		 {
		 this->ptr = &win;
		 }
		 Button win;
		 };*/
		langObject button_create(std::vector<langObject> arg)
		{
			ObjectWin* parent = dynamic_cast<ObjectWin*>(arg[0]);
			ObjectBtn* a = new ObjectBtn();
			std::string str2 = arg[1]->toString();
			//int len = str2.length();
			//TCHAR* buf = new TCHAR[len * 2];
			//buf[MultiByteToWideChar(CP_OEMCP,MB_PRECOMPOSED,str2.c_str(),len,buf,len * 2)] = _T('\0');
			a->win = new Button(*parent->win, str2.c_str(), Int::toInt(arg[2]), Int::toInt(arg[3]), Int::toInt(arg[4]), Int::toInt(arg[5]));
			//delete buf;
			//OLWindow::windowmap[a->win->hWnd] = &a->win;
			a->win->Tag = a;
			a->win->SetFont("MS UI Gothic", 9);
			return a;
		}
		langObject label_create(std::vector<langObject> arg)
		{
			ObjectWin* parent = dynamic_cast<ObjectWin*>(arg[0]);
			ObjectLabel* a = new ObjectLabel();
			std::string str2 = arg[1]->toString();
			//int len = str2.length();
			//TCHAR* buf = new TCHAR[len * 2];
			//buf[MultiByteToWideChar(CP_OEMCP,MB_PRECOMPOSED,str2.c_str(),len,buf,len * 2)] = _T('\0');
			a->win = new Label(*parent->win, str2.c_str(), Int::toInt(arg[2]), Int::toInt(arg[3]), Int::toInt(arg[4]), Int::toInt(arg[5]));
			//delete buf;
			//OLWindow::windowmap[a->win->hWnd] = &a->win;
			a->win->Tag = a;
			a->win->SetFont("MS UI Gothic", 9);
			return a;
		}
		langObject textbox_create(std::vector<langObject> arg)
		{
			ObjectWin* parent = dynamic_cast<ObjectWin*>(arg[0]);
			ObjectTextBox* a = new ObjectTextBox();
			std::string str2 = arg[1]->toString();
			//int len = str2.length();
			//TCHAR* buf = new TCHAR[len * 2 + 1];
			//buf[MultiByteToWideChar(CP_OEMCP,MB_PRECOMPOSED,str2.c_str(),len,buf,len * 2)] = _T('\0');
			a->win = new TextBox(*parent->win, str2.c_str(), Int::toInt(arg[2]), Int::toInt(arg[3]), Int::toInt(arg[4]), Int::toInt(arg[5]), Int::toInt(arg[6]));
			//delete buf;
			//OLWindow::windowmap[a->win->hWnd] = &a->win;
			a->win->Tag = a;
			a->win->SetFont("MS UI Gothic", 9);
			return a;
		}
		langObject textbox_setreadonly(std::vector<langObject> arg)
		{
			ObjectTextBox* textbox = dynamic_cast<ObjectTextBox*>(arg[0]);
			textbox->win->SetReadOnly(Int::toInt(arg[1]));
			return NULLOBJECT;
		}
		langObject textbox_setmultiline(std::vector<langObject> arg)
		{
			ObjectTextBox* textbox = dynamic_cast<ObjectTextBox*>(arg[0]);
			textbox->win->SetMultiLine(Int::toInt(arg[1]));
			return NULLOBJECT;
		}
		langObject textbox_getreadonly(std::vector<langObject> arg)
		{
			ObjectTextBox* textbox = dynamic_cast<ObjectTextBox*>(arg[0]);
			return newInt(textbox->win->GetReadOnly());
		}
		langObject textbox_getmultiline(std::vector<langObject> arg)
		{
			ObjectTextBox* textbox = dynamic_cast<ObjectTextBox*>(arg[0]);
			return newInt(textbox->win->GetMultiLine());
		}
		langObject checkbox_create(std::vector<langObject> arg)
		{
			ObjectWin* parent = /*dynamic*/static_cast<ObjectWin*>(arg[0]);
			ObjectCheckBox* a = new ObjectCheckBox();
			std::string str2 = arg[1]->toString();
			//int len = str2.length();
			//TCHAR* buf = new TCHAR[len * 2];
			//buf[MultiByteToWideChar(CP_OEMCP,MB_PRECOMPOSED,str2.c_str(),len,buf,len * 2)] = _T('\0');
			a->win = new CheckBox(*parent->win, str2.c_str(), Int::toInt(arg[2]), Int::toInt(arg[3]), Int::toInt(arg[4]), Int::toInt(arg[5]));
			//delete buf;
			//OLWindow::windowmap[a->win->hWnd] = &a->win;
			a->win->Tag = a;
			a->win->SetFont("MS UI Gothic", 9);
			return a;
		}
		void window_setonclick_event(eventargs<OLWindow*> v)
		{
			ObjectWin* a = static_cast<ObjectWin*>(v.Arg->Tag);
			std::vector<langObject> arg;
			arg.push_back(a);
			a->Event["OnClick"]->call(&arg);
		}
		langObject window_setonclick(std::vector<langObject> arg)
		{
			ObjectWin* a = (ObjectWin*)arg[0];//dynamic_cast<ObjectWin*>(arg[0]);
			a->Event["OnClick"] = (langFunction)arg[1];
			a->win->OnClick += window_setonclick_event/*[](eventargs<OLWindow*> v)
													 {
													 ObjectWin* a = static_cast<ObjectWin*>(v.Arg->Tag);
													 std::vector<langObject> arg;
													 arg.push_back(a);
													 a->Event["OnClick"]->call(&arg);
													 }*/;
			return a;
		}
		langObject window_setfont(std::vector<langObject> arg)
		{
			ObjectWin* a = (ObjectWin*)arg[0];
			std::string str2 = arg[1]->toString();
			//int len = str2.length();
			//TCHAR* buf = new TCHAR[len * 2];
			//buf[MultiByteToWideChar(CP_OEMCP,MB_PRECOMPOSED,str2.c_str(),len,buf,len * 2)] = _T('\0');
			if (arg.size() == 2) a->win->SetFont(str2.c_str(), Int::toInt(arg[1]));
			else if (arg.size() == 7) a->win->SetFont(str2.c_str(), Int::toInt(arg[2]), Int::toInt(arg[3]), Int::toInt(arg[4]), Int::toInt(arg[5]), Int::toInt(arg[6]));
			return a;
		}
		langObject messagebox(std::vector<langObject> arg)
		{
			if (arg.size() == 1)
				__MessageBox(arg[0]->toString().c_str());//MessageBoxA(NULL, arg[0]->toString().c_str(),"",MB_OK);
			else
			if (arg.size() == 2)
			{
				ObjectWin* window = dynamic_cast<ObjectWin*>(arg[1]);
				__MessageBox(*window->win, arg[0]->toString().c_str());//MessageBoxA(window->win->hWnd, arg[0]->toString().c_str(),"",MB_OK);
			}
			return NULLOBJECT;
		}
#ifdef CPP11
		langObject olruntime_gc_asyncgc(std::vector<langObject> arg)
		{
			std::thread* thd = new std::thread([]{lang::gc->start(); });
			return NULLOBJECT;
		}
#endif
		langObject exit(std::vector<langObject> arg)
		{
			if (arg.size())
				throw langExitException(Int::toInt(arg[0]));
			else
				throw langExitException(0);//::exit(0);
		}
		langObject time(std::vector<langObject> arg)
		{
			return newInt(::time(NULL));
		}
		langObject getstack(std::vector<langObject> arg)
		{
			std::vector<langObject> *stack = (std::vector<langObject>*)lang::stacktrace;
			langArray ary = newArray(0);
			ary->ary = *stack;
			if (arg.size() > 0)
			{
				int j = Int::toInt(arg[0]);
				for (int i = 0; i < j; i++)
				{
					ary->ary.pop_back();
				}
			}
			return ary;
		}
		extern "C"
		{
			typedef int(__cdecl  *dynamicfunc)(void);
		}
		langObject dynamiccall(std::vector<langObject> arg)
		{
			HMODULE hDll;
			std::string str = arg[0]->toString();
			std::string funcstr = arg[1]->toString();
			hDll = LoadLibraryA(str.c_str());
			if (hDll == NULL) {
				throw_langRuntimeException("fail LoadLibraryA(\"%s\")", str.c_str());
			}
			dynamicfunc test = (dynamicfunc)GetProcAddress(hDll, funcstr.c_str());
			if (test == NULL) {
				throw_langRuntimeException("fail GetProcAddress(\"%s\")", funcstr.c_str());
			}
			int j;
			double dj;
			char cj;
			wchar_t wj;
			const char* sj;
			int result;
			if (arg.size() == 3 && arg[2]->type->TypeEnum == _Int)
			{
				j = Int::toInt(arg[2]);
				__asm
				{
					mov eax, j;
				}
				test();
				__asm
				{
					mov result, eax;
				}
				return newInt(result);
			}
			for (int i = arg.size() - 1; i >= 2; i--)
				switch (arg[i]->type->TypeEnum)
				{
					case _Int:
						j = Int::toInt(arg[i]);//Int::toInt(arg[i]);
						__asm
						{
							push j;
						}
						break;
					case _Double:
						dj = Double::toDouble(arg[i]);//Int::toInt(arg[i]);
						__asm
						{
							push dj;
						}
						break;
					case _Char:
						cj = Char::toChar(arg[i]);//Int::toInt(arg[i]);
						__asm
						{
							push cj;
						}
						break;
					case _WChar:
						wj = WChar::toWChar(arg[i]);//Int::toInt(arg[i]);
						__asm
						{
							push wj;
						}
						break;
					case _String:
						sj = ((langString)arg[i])->getString()->c_str();
						__asm
						{
							push sj;
						}
						break;
			}
			result = test();
			/*__asm
			{
				mov result, eax;
			}*/
			for (int i = arg.size() - 1; i >= 2; i--)
			__asm
			{
				pop j;
			}
			return newInt(result);
		}
	}
}
