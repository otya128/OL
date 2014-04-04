#include "stdafx.h"
#include <cmath>
#include <stdlib.h>
#include <sstream>
#include <iosfwd>
#include "Function.h"
#include "OLR.h"
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
		void Add(std::string name, BuiltFunc func)
		{
			(*lang::BuiltFunction)[name] = func;
		}
		void init()
		{
			Add("print", print);
			Add("getline", getline);
			Add("GC", GC);
			Add("free", free);
			Add("uncontrollGC", uncontrollGC);
			Add("VarPtr", VarPtr);
			Add("WriteMemory", WriteMemory);
			Add("ReadMemoryInt", ReadMemoryInt);
			Add("sqrt", sqrt);
#ifdef CPP11
			Add("thread", thread);
			Add("threadid", threadid);
			Add("thread::join", thread_join);
#endif
			Add("File::open", Fopen);
			Add("File::close", Fclose);
			Add("File::write", Fwrite);
			Add("File::stdin", getstdin);
			Add("File::stdout", getstdout);
			Add("File::stderr", getstderr);
			Add("OLRuntime::GC::ObjectCount", olruntime_gc_objectcount);
#ifdef CPP11
			Add("OLRuntime::GC::BackgroundGC", olruntime_gc_asyncgc);
#endif
			Add("OLRuntime::GUI::Window::Create", window_create);
			Add("OLRuntime::GUI::Window::Show", window_show);
			Add("OLRuntime::GUI::Button::Create", button_create);
			Add("OLRuntime::GUI::Window::SetOnClick", window_setonclick);
			Add("OLRuntime::GUI::Window::SetFont", window_setfont);
			Add("OLRuntime::GUI::Window::GetText", window_gettext);
			Add("OLRuntime::GUI::Window::SetText", window_settext);
			Add("OLRuntime::GUI::Label::Create", label_create);
			Add("OLRuntime::GUI::TextBox::Create", textbox_create);
			Add("OLRuntime::GUI::TextBox::SetReadOnly", textbox_setreadonly);
			Add("OLRuntime::GUI::TextBox::GetReadOnly", textbox_getreadonly);
			Add("OLRuntime::GUI::TextBox::SetMultiLine", textbox_setmultiline);
			Add("OLRuntime::GUI::TextBox::GetMultiLine", textbox_getmultiline);
			Add("OLRuntime::GUI::CheckBox::Create", checkbox_create);
			Add("OLRuntime::GUI::MessageBox", messagebox);
			Add("exit", exit);
			Add("time", time);
			Add("OLRuntime::GetStack", getstack);
			Add("dynamiccall", dynamiccall);
			Add("tonativefunc", tonativefunc);
			Add("sizeof", _sizeof);
		}
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
	}
	int object_distance(langObject m, langObject n);
	namespace lib
	{
		langObject VarPtr(std::vector<langObject> arg)
		{
			void* i;
			if (arg[0]->type->TypeEnum == _ClassObject && object_distance(arg[0], lang::ClassArrayBufferClass) != INT_MAX)
			{
				i = ArrayBufferGetPointer(((lang::ArrayBufferClassObject*)arg[0]));
				return newInt(*reinterpret_cast<int*>(&i));
			}
			i = arg[0]->getPointer();
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
			langFunction threadFunc = Function::CopyFunction((langFunction)arg[0], ((langFunction)arg[0])->thisscope ? ((langFunction)arg[0])->thisscope : ((langFunction)arg[0])->scope);
			std::thread* thd = new std::thread([threadFunc]
			{
				try
				{
					running += 1;
					lang::stacktrace = new std::vector<langFunction>();
					std::vector<langObject> rarg;
					threadFunc->call(&rarg);
					threadFunc->thread->detach();
					lang::gc->free_(threadFunc);
				}
				catch (...)
				{
					if (threadFunc)std::cerr << "Exception" << threadFunc->toString();
				}
				running -= 1;
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
			typedef int(__stdcall  *dynamicfunc)(void);
		}
		struct rettype
		{
			union
			{
				int i;
				int hl[2];
				long long hlhl;
				double d;
				char c;
				wchar_t w;
				void* p;
			};
		};
		ArrayBufferClassObject *getArrayBufferClassObject(langClassObject cc)
		{
			while (cc)
			{
				if (cc->staticClass == lang::ClassArrayBufferClass)
				{
					return (ArrayBufferClassObject*)cc;
				}
				cc = (langClassObject)cc->base;
			}
		}
		enum callruleenum
		{
			stdcall_,
			cdecl_,
			pascal_,
			fastcall_,
		};
		langObject dynamiccall(std::vector<langObject> arg)
		{
			callruleenum callrule = stdcall_;
			size_t _ESP;
			__asm
			{
				mov _ESP, esp;
			}
			HMODULE hDll;
			std::string str = arg[0]->toString();
			int retsize = 4;
			ObjectType* type = (ObjectType*)IntTypeObject;
			lang::ArrayBufferClassObject* arraybuf = false;
			bool is_va_arg = false;
			if (arg[1]->type->TypeEnum == _Type)
			{
				type = (ObjectType*)arg[1];
				switch (type->TypeClass.TypeEnum)
				{
					case _Int:
						retsize = sizeof(int);
						break;
					case _Double:
						retsize = sizeof(double);
						break;
					case _Char:
						retsize = sizeof(char);
						break;
					case _WChar:
						retsize = sizeof(wchar_t);
						break;
					case _String:
						retsize = sizeof(char*);
						break;
				}
				arg.erase(arg.begin()++);
			}
			else if (arg[1]->type->TypeEnum == _ClassObject)
			{
				if (object_distance(arg[1], lang::ClassArrayBufferClass) != INT_MAX)
				{
					auto cc = (langClassObject)arg[1];
					while (cc)
					{
						if (cc->staticClass == lang::ClassArrayBufferClass)
						{
							retsize = ArrayBufferGetSize(((lang::ArrayBufferClassObject*)cc));
							arraybuf = ((lang::ArrayBufferClassObject*)arg[1]);
						}
						cc = (langClassObject)cc->base;
					}
				}
				else
					throw langRuntimeException("OL::ArrayBufferを継承したクラスのみ使えます[未実装:ArrayBufferable]");
				arg.erase(arg.begin()++);
			}
			std::string funcstr = arg[1]->toString();
			{
				int ret = funcstr.find(':');
				if (ret != -1)
				{
					if (funcstr.substr(ret + 1) == "va_arg")
					{
						is_va_arg = true;
					}
					if (funcstr.substr(ret + 1) == "cdecl")
					{
						callrule = cdecl_;
					}
					funcstr = funcstr.substr(0, ret);
				}
			}
			hDll = LoadLibraryA(str.c_str());
			if (hDll == NULL) {
				throw_langRuntimeException("fail LoadLibraryA(\"%s\")", str.c_str());
			}

			dynamicfunc test_ = (dynamicfunc)GetProcAddress(hDll, funcstr.c_str());
			if (test_ == NULL) {
				throw_langRuntimeException("fail GetProcAddress(\"%s\")", funcstr.c_str());
			}
			int j;
			volatile double dj;
			char cj;
			wchar_t wj;
			const char* sj;
			int absiz;
			int *ab;
			void *resultaddr = nullptr;
			int stacksiz = 0;
			if ((retsize >= 5 && retsize <= 7) || retsize >= 9)
			{
				resultaddr = new char[retsize];
			}
			rettype result;
			langObject resultobj = NULLOBJECT;
			/*if (arg.size() == 3 && arg[2]->type->TypeEnum == _Int)
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
			}*/
			for (int i = arg.size() - 1; i >= 2; i--)
				switch (arg[i]->type->TypeEnum)
			{
					case _Int:
						j = Int::toInt(arg[i]);//Int::toInt(arg[i]);
						__asm
						{
							push j;
						}
						stacksiz += 4;
						break;
					case _Double:
						dj = Double::toDouble(arg[i]);//Int::toInt(arg[i]);
						__asm
						{
							sub         esp, 8;
							movsd       xmm0, mmword ptr[dj];
							movsd       mmword ptr[esp], xmm0;
							; push dj;
							; push dj;
							; movss       xmm0, dj; XMM00 = 1.0
								; movss       dword ptr[esp], xmm0; *esp = XMM00
						}
						stacksiz += 8;
						break;
					case _Char:
						cj = Char::toChar(arg[i]);//Int::toInt(arg[i]);
						__asm
						{
							push cj;
						}
						stacksiz += 4;
						break;
					case _WChar:
						wj = WChar::toWChar(arg[i]);//Int::toInt(arg[i]);
						__asm
						{
							push wj;
						}
						stacksiz += 2;
						break;
					case _String:
						sj = ((langString)arg[i])->getString()->c_str();
						__asm
						{
							push sj;
						}
						stacksiz += 4;
						break;
					case _ClassObject:
						if (object_distance(arg[i], lang::ClassArrayBufferClass) != INT_MAX)
						{
							absiz = ArrayBufferGetSize(((lang::ArrayBufferClassObject*)arg[i]));
							absiz += absiz % 4;
							ab = (((int *)ArrayBufferGetPointer((lang::ArrayBufferClassObject*)arg[i]))) + (absiz / 4) - 1;
							for (j = absiz; j > 0; j -= 4)
							{
								resultaddr = (void*)*ab;
								__asm
								{
									push resultaddr;
								}
								ab--;
								stacksiz += 4;
							}
						}
						else
							throw langRuntimeException("OL::ArrayBufferを継承したクラスのみ使えます[未実装:ArrayBufferable]");
						break;
			}
			resultaddr = nullptr;
			if (retsize > 8)
			{
				auto ret = arraybuf->CreateObject(arraybuf->staticClass);
				arg.clear();
				ArrayBufferSetPointer(((ArrayBufferClassObject*)ret), new char[retsize]);
				ArrayBufferSetSize(((ArrayBufferClassObject*)ret), retsize);
				resultaddr = ret;
			}
			switch (retsize)
			{
				case 1:
					__asm
					{
						call test_;
						mov result.c, al;
					}
					resultobj = newChar(result.c);
					break;
				case 2:
					__asm
					{
						call test_;
						mov result.w, ax;
					}
					resultobj = new WChar(result.w);
					break;
				case 4:
					__asm
					{
						call test_;
						mov result.i, eax;
					}
					resultobj = newInt(result.i);
					//result.i = test_();
					break;
				case 8:
					__asm
					{
						call test_;
						mov result.hl[0], eax;
						mov result.hl[1], edx;
					}
					break;
				default:
					__asm
					{
						push resultaddr;
						call test_;
					}
					break;
			}
			if (type->TypeClass.TypeEnum == _String)
			{
				resultobj = newString((char*)result.i);
			}
			if (type->TypeClass.TypeEnum == _Int)
			{
				resultobj = newInt(result.i);
			}
			if (type->TypeClass.TypeEnum == _Double)
			{
				__asm
				{
					fstp qword ptr[result.d];
				}
				resultobj = newDouble(result.d);
			}
			if (type->TypeClass.TypeEnum == _Char)
			{
				resultobj = newChar(result.c);
			}
			if (type->TypeClass.TypeEnum == _WChar)
			{
				resultobj = new WChar(result.w);
			}
			/*__asm
			{
			mov result, eax;
			}*/
			if (is_va_arg || callrule == cdecl_)
			{
				__asm
				{
					add esp, stacksiz;
				}
			}
			if (arraybuf)
			{
				auto ret = arraybuf->CreateObject(arraybuf->staticClass);
				arg.clear();
				if (resultaddr)
				{
					ArrayBufferSetNonUnique(((ArrayBufferClassObject*)ret), true);
					ArrayBufferSetPointer(((ArrayBufferClassObject*)ret), resultaddr);
				}
				else
				{
					ArrayBufferSetPointer(((ArrayBufferClassObject*)ret), new char[retsize]);
					auto saet = (langFunction)ret->getMember("bracketequal", ret->thisscope);
					if (saet is _Function)
					{
						arg.clear();
						arg.push_back(FALSEOBJECT);
						switch (retsize)
						{
							case 8:
								arg.push_back(newInt(result.hl[0]));
								saet->call(&arg);
								arg.clear();
								arg.push_back(TRUEOBJECT);
								arg.push_back(newInt(result.hl[1]));
								break;
							default:
								arg.push_back(resultobj);
								break;
						}
						ArrayBufferSetSize(((ArrayBufferClassObject*)ret), retsize);
						saet->call(&arg);
					}
				}
				resultobj = ret;

			}
			/*for (int i = arg.size() - 1; i >= 2; i--)
		__asm
		{
		pop j;
		}*/
			size_t _ESP2;
			__asm
			{
				mov _ESP2, esp;
			}
			if (_ESP != _ESP2)
			{
				std::cerr << "error 不正なESP 引数か呼び出し規則間違ってるかも 可変長引数なら:va_argを付ける" << std::endl;
				_asm
				{
					mov esp, _ESP;
				}
			}
			return resultobj;
		}
		size_t _sizeof(langObject arg)
		{
			int retsize = 1;
			switch (arg->type->TypeEnum)
			{
				case _Int:
					retsize = sizeof(int);
					break;
				case _Double:
					retsize = sizeof(double);
					break;
				case _Char:
					retsize = sizeof(char);
					break;
				case _WChar:
					retsize = sizeof(wchar_t);
					break;
				case _String:
					retsize = sizeof(char*);
					break;
				case _ClassObject:
					if (object_distance(arg, lang::ClassArrayBufferClass) != INT_MAX)
					{
						retsize = ArrayBufferGetSize(((lang::ArrayBufferClassObject*)arg));
						break;
					}
				default:
					retsize = sizeof(size_t);
			}
			return (retsize);
		}
		langObject _sizeof(std::vector<langObject> arg)
		{
			return newInt(_sizeof(arg[0]));
		}
		langObject tonativefunc(std::vector<langObject> arg)
		{
			nativefunc a = ToNativeFunc(arg);
			ArrayBufferClassObject* ret = new ArrayBufferClassObject();
			ArrayBufferSetPointer(ret, a);
			ArrayBufferSetSize(ret, 4);
			return ret;
		}
		const char basefunc1[] =
		{
			0x55, 0x8b, 0xec, 0x81, 0xec, 0xc0, 0x00, 0x00, 0x00, 0x53, 0x56, 0x57, 0x8d, 0xbd, 0x40, 0xff, 0xff, 0xff, 0xb9, 0x30, 0x00, 0x00, 0x00, 0xb8, 0xcc, 0xcc, 0xcc, 0xcc, 0xf3, 0xab,
		};
		const char basefunc2[] =
		{
			//0x6a, 0x01,//push 1
			//0x6a, 0x01,//push 1
			//0xa1, 0x70, 0x0e, 0x20, 0x00,//mov         eax,dword ptr ds:[00200E70h]//関数
			//0x50,//push eax
			//0x8b, 0x0d, 0xc4, 0x0e, 0x20, 0x00,//mov         ecx,dword ptr ds:[200EC4h]  //返り値
			//0x51,//push ecx
			//0xe8, 0x26, 0xf8, 0xf6, 0xff,//call        lang::lib::lap (0EBF6Ah)  //ここも動的に変える
			//0x83, 0xc4, 0x10,// add         esp,10h 
			0x5f, 0x5e, 0x5b, 0x81, 0xc4, 0xc0, 0x00,
			0x00, 0x00, /*0x3b, 0xec, /*0xe8, 0xa9, 0xee, 0xf6, 0xff,*/ 0x8b, 0xe5, 0x5d, 0xc3,
		};
		const unsigned char PUSH_EAX = 0x50;
		const unsigned char PUSH_ECX = 0x51;
		void lap(langObject ret, langFunction f, int, ...);
		nativefunc ToNativeFunc(std::vector<langObject> arg)
		{
			struct exception_mendo{ bool success; unsigned char* function; exception_mendo(){ success = false; function = new unsigned char[512]; memcpy(function, basefunc1, sizeof(basefunc1)); }~exception_mendo(){ if (!success)delete function; } }
			func;
			size_t ptr;
			unsigned char* function = func.function;//new unsigned char[512];]
			//argは8から始まるらしひmov     eax, [ebp+arg_4]
			unsigned char* body = function + sizeof(basefunc1);
			if (arg.size() != 2)
			{
				throw langRuntimeException("引数の数がToNativeFunc");
			}
			if (arg[0] is _Function && (arg[1] is _Type || arg[1] == NULLOBJECT || arg[1] is _ClassObject))
			{
				int arg_stack = 0x08;// +4 * 3;
				if (_sizeof(arg[1]) > 8)
				{
					//隠しパラメータが来るのでarg_stackを増やしておく
					//*body = 0x8b; body++;						//mov eax
					//*body = 0x45; body++;						//[ebp +
					//*body = (unsigned char)arg_stack; body++;	//       0]
					//*body = PUSH_EAX; body++;
					//arg_stack += 4;
				}
				langFunction f = (langFunction)arg[0];
				//*body = 0xA1; body++;//mov eax,[imm]
				//TODO:32bit ptr
				for (int i = f->argList->size() - 1; i >= 0; i--)
				{
					std::string &type = f->argList->at(i).first;
					langObject typeo = f->scope->variable(type, f->scope);
					if (typeo is _Type || typeo is _ClassObject)
					{
						if (object_distance(typeo, lang::ClassArrayBufferClass) != INT_MAX)
						{
							if (typeo is _ClassObject)
							{
								size_t absiz = ArrayBufferGetSize(((lang::ArrayBufferClassObject*)typeo));
								absiz += absiz % 4;
								//逆さにする必要があったので最初から逆で
								arg_stack += absiz;
								for (int j = absiz; j > 0; j -= 4)
								{
									*body = 0x8b; body++;						//mov eax
									*body = 0x45; body++;						//[ebp +
									*body = (unsigned char)arg_stack; body++;	//       0]
									*body = PUSH_EAX; body++;
									arg_stack -= 4;
								}
								arg_stack += absiz;
								//for (int j = absiz; j > 0; j -= 4)
								//{
								//	*body = 0x8b; body++;						//mov eax
								//	*body = 0x45; body++;						//[ebp +
								//	*body = (unsigned char)arg_stack; body++;	//       0]
								//	*body = PUSH_EAX; body++;
								//	arg_stack += 4;
								//}
							}
							else
							{
								throw langRuntimeException("OL::ArrayBufferを継承したクラスのみ使えます[未実装:ArrayBufferable]");
							}
						}
						else
						{
							if (((ObjectType*)typeo)->TypeClass.TypeEnum == _Double)
							{
								*body = 0x8b; body++;						//mov eax
								*body = 0x45; body++;						//[ebp +
								*body = (unsigned char)arg_stack; body++;	//       0]
								*body = PUSH_EAX; body++;
								arg_stack += 4;
							}
							//*body = 
							//引数をpush
							*body = 0x8b; body++;						//mov eax
							*body = 0x45; body++;						//[ebp +
							*body = (unsigned char)arg_stack; body++;	//       0]
							*body = PUSH_EAX; body++;
							arg_stack += 4;
						}

						//(1,int,2,int)=>cdecl
						//(int,2,int,1)
						//typeをpush
						//よく考えたらいらない！！！！！！->FunctionのArgListに型があるただ遅いかもしれないので結局そのまま
						*body = 0xB8/*0xA1*/; body++;//mov eax,[imm]
						//TODO:32bit ptr
						ptr = (size_t)typeo;
						for (int i = 0; i < 4; i++)
						{
							*body = ptr & 0xFF;
							ptr = ptr >> 8;
							body++;// >>
						}
						*body = PUSH_EAX; body++;
						arg_stack += 4;
						//隠しパラメータ
						//8byteを超えると
						if (_sizeof(arg[1]) > 8)
						{
							*body = 0x8b; body++;						//mov eax
							*body = 0x45; body++;						//[ebp +
							*body = (unsigned char)/*arg_stack*/0x8; body++;	   //       0]
							*body = PUSH_EAX; body++;
							arg_stack += 4;
						}
					}
					else
					{
						throw langRuntimeException("関数の引数に型が設定されていません。");
					}
				}
				//ArgListの大きさを引数に入れる
				*body = 0xB8/*0xA1*/; body++;//mov eax,[imm]
				*body = (unsigned char)f->argList->size(); body++;
				*body = 0; body++;
				*body = 0; body++;
				*body = 0; body++;
				*body = PUSH_EAX; body++; arg_stack += 4;
				//*body = PUSH_EAX; body++; arg_stack += 4;
				//関数を引数に入れる
				ptr = (size_t)f;
				*body = 0xB8/*0xA1*/; body++;//mov eax,[imm]
				for (int i = 0; i < 4; i++)
				{
					*body = ptr & 0xFF;
					ptr = ptr >> 8;
					body++;// >>
				}
				*body = PUSH_EAX; body++; arg_stack += 4;
				//戻り値を引数に入れる
				ptr = (size_t)arg[1];
				*body = 0xB8/*0xA1*/; body++;//mov eax,[imm]
				for (int i = 0; i < 4; i++)
				{
					*body = ptr & 0xFF;
					ptr = ptr >> 8;
					body++;// >>
				}
				*body = PUSH_EAX; body++; arg_stack += 4;
				//call
#ifdef ZETTAI
				*body = 0x9A; body++;
				ptr = (size_t)(lap);
				for (int i = 0; i < 4; i++)
				{
					*body = (unsigned char)(ptr & 0xFF);
					ptr = ptr >> 8;
					body++;// >>
				}
				//TODO:32bit ptr
				//body += 3;
				short _CS;
				__asm
				{
					mov _CS, CS; DS;
				}
				for (int i = 0; i < 2; i++)
				{
					*body = (unsigned char)(_CS & 0xFF);
					_CS = _CS >> 8;
					body++;// >>
				}
#else

				*body = 0xE8; body++;
				ptr = (size_t)(lap);
				ptr = (size_t)(lap)-(size_t)(body)-4;// + 6;
				for (int i = 0; i < 4; i++)
				{
					*body = (unsigned char)(ptr & 0xFF);
					ptr = ptr >> 8;
					body++;// >>
				}
				//TODO:32bit ptr
				//body += 3;
#endif
				//body += 5;
				*body = 0x83; body++;//add
				*body = 0xC4; body++;//esp
				*body = (unsigned char)arg_stack - 0x08; body++;
				func.success = true;
				memcpy(body, basefunc2, sizeof(basefunc2));
				return (void(*)(void))function;
			}
			else
				throw langRuntimeException("引数がFunctionじゃない");
		}
		void cdecl lap(langObject ret, langFunction f, int va_size, ...)
		{
			rettype i;
			i.hlhl = 0;
			va_list args;
			va_start(args, va_size);
			std::vector<langObject> arg;
			langObject obj;
			ObjectType* type;
			langClassObject isarybuf = false;
			langClassObject argarybuf = false;
			if (f->isoverload())
			{
				std::cerr << "Overload????\n";
			}
			int j;
			int n;
			void*ptr;
			void*ptrret = va_arg(args, void*);
			//(void *)((args += ((16 + sizeof(int)-1) & ~(sizeof(int)-1)) - ((16 + sizeof(int)-1) & ~(sizeof(int)-1))));
			for (j = 0; j < va_size; j++)
			{
				obj = va_arg(args, langObject);
				if (obj is _Type)
				{
					type = (ObjectType*)obj;
					switch (type->TypeClass.TypeEnum)
					{
						case _Int:
							arg.push_back(newInt(va_arg(args, int)));
							break;
						case _Double:
							arg.push_back(newDouble(va_arg(args, double)));
							break;
						case _String:
							arg.push_back(newString(va_arg(args, char*)));
							break;
						case _Char:
							arg.push_back(newChar(va_arg(args, char)));
							break;
						case _WChar:
							arg.push_back(new WChar(va_arg(args, wchar_t)));
							break;
					}
				}
				if (obj is _ClassObject)
				{
					n = ArrayBufferGetSize((langClassObject)obj);
					argarybuf = ((langClassObject)obj)->staticClass->CreateObject(ClassArrayBufferClass);
					ArrayBufferSetPointer(argarybuf, /***/(void *)args);//((n + sizeof(int)-1) & ~(sizeof(int)-1)) - ((n + sizeof(int)-1) & ~(sizeof(int)-1)))));
					((args += n));
					ArrayBufferSetSize(argarybuf, n);
					ArrayBufferSetNonUnique(argarybuf, true);//freeしない
					arg.push_back(argarybuf);
				}
			}
			if (ret->type->TypeEnum == _ClassObject)
			{
				argarybuf = (langClassObject)f->call(&arg);
				n = ArrayBufferGetSize(argarybuf);
				ptr = ArrayBufferGetPointer(argarybuf);
				if (n > 8)
				{
					//va_arg(args, void*);
					memcpy(/**(int**)args*/ptrret, ptr, n);
					va_end(args);
					__asm
					{
						jmp end;
					}
				}
				else
				{
					memcpy(&i, ptr, n);
					if (n > 4)
					{
						__asm{mov eax, i.hl[0]
							mov edx, i.hl[1]}__asm{jmp end}
					}
					__asm{mov eax, i.i }__asm{jmp end}
				}
			}
			if (ret->type->TypeEnum == _Type)
			{
				switch (((ObjectType*)ret)->TypeClass.TypeEnum)
				{
					case _Int:
						i.i = Int::toInt(f->call(&arg));
						__asm
						{
						iend:
							; m ov eax, i.i;
						end:
							mov         dword ptr[args], 0;
							mov         dword ptr[ebp - 4], 0FFFFFFFFh;
							lea         ecx, [arg];
							push eax;
							call        std::vector<lang::Object *, std::allocator<lang::Object *> >::~vector<lang::Object *, std::allocator<lang::Object *> >;
							pop eax;
							mov         dword ptr fs : [0], ecx;
							pop         ecx;
							pop         edi;
							pop         esi;
							pop         ebx;
							add         esp, 1E8h;
							mov         esp, ebp;
							pop         ebp;
							ret;
						}
					case _Char:
						i.c = Char::toChar(f->call(&arg));
						__asm{mov eax, i.i }__asm{jmp end}
					case _Double:
						i.d = Double::toDouble(f->call(&arg));
						//__asm{mov eax, i.d }
						__asm{jmp end}
					case _WChar:
						i.w = WChar::toWChar(f->call(&arg));
						__asm{mov eax, i.i }__asm{jmp end}
					case _String:
						i.p = f->call(&arg)->getPointer();
						__asm{mov eax, i.i }__asm{jmp end}
				}
			}
			va_end(args);
		}
		langFunction dmmytestDEBUF;
		void tonativefuncbase(int i, int j)
		{
			return lap(lang::IntTypeObject, dmmytestDEBUF, i, j);
			///std::vector<langObject> arg;
			//arg.push_back(TRUEOBJECT);
			//dmmytestDEBUF->call(&arg);
			////
			//__asm
			//{
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//	nop;
			//}
		}
	}
}
