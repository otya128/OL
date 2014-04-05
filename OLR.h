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
		langObject tonativefunc(std::vector<langObject> arg);
#endif
		langObject exit(std::vector<langObject> arg);
		langObject time(std::vector<langObject> arg);
		langObject getstack(std::vector<langObject> arg);
		langObject _sizeof(std::vector<langObject> arg);
		langObject PNativeFuncCall(std::vector<langObject> arg);
		typedef void(*nativefunc)(void);
		nativefunc __stdcall ToNativeFunc(std::vector<langObject> arg);
		void init();
	}
}
#endif
