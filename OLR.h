#ifndef __LANG_OLR_H__
#define __LANG_OLR_H__
#include "stdafx.h"
#include "Object.h"
namespace lang
{
	typedef langObject(*BuiltFunc)(std::vector<langObject>);
	extern std::map<std::string, BuiltFunc>* BuiltFunction;
	namespace lib
	{
		langObject print(std::vector<langObject> arg);
		langObject getline(std::vector<langObject> arg);
		langObject GC(std::vector<langObject> arg);
		langObject free(std::vector<langObject> arg);
		langObject uncontrollGC(std::vector<langObject> arg);
		langObject VarPtr(std::vector<langObject> arg);
		langObject WriteMemory(std::vector<langObject> arg);
		langObject ReadMemoryInt(std::vector<langObject> arg);
		langObject sqrt(std::vector<langObject> arg);
#ifdef CPP11
		langObject thread(std::vector<langObject> arg);
		langObject thread_join(std::vector<langObject> arg);
		langObject threadid(std::vector<langObject> arg);
#endif
		langObject Fopen(std::vector<langObject> arg);
		langObject Fclose(std::vector<langObject> arg);
		langObject Fwrite(std::vector<langObject> arg);
		langObject getstdin(std::vector<langObject> arg);
		langObject getstdout(std::vector<langObject> arg);
		langObject getstderr(std::vector<langObject> arg);
		langObject olruntime_gc_objectcount(std::vector<langObject> arg);
		langObject olruntime_gc_gctiming(std::vector<langObject> arg);
		langObject olruntime_gc_run(std::vector<langObject> arg);
		langObject window_create(std::vector<langObject> arg);
		langObject window_show(std::vector<langObject> arg);
		langObject window_gettext(std::vector<langObject> arg);
		langObject window_settext(std::vector<langObject> arg);
		langObject button_create(std::vector<langObject> arg);
		langObject label_create(std::vector<langObject> arg);
		langObject textbox_create(std::vector<langObject> arg);
		langObject textbox_setreadonly(std::vector<langObject> arg);
		langObject textbox_setmultiline(std::vector<langObject> arg);
		langObject textbox_getreadonly(std::vector<langObject> arg);
		langObject textbox_getmultiline(std::vector<langObject> arg);
		langObject checkbox_create(std::vector<langObject> arg);
		langObject window_setonclick(std::vector<langObject> arg);
		langObject window_setfont(std::vector<langObject> arg);
		langObject messagebox(std::vector<langObject> arg);
#ifdef CPP11
		langObject olruntime_gc_asyncgc(std::vector<langObject> arg);
#endif
#ifdef _WIN32
		langObject dynamiccall(std::vector<langObject> arg);
#endif
		langObject exit(std::vector<langObject> arg);
		langObject time(std::vector<langObject> arg);
		langObject getstack(std::vector<langObject> arg);
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
		}
	}
}
#endif
