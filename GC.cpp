#include "stdafx.h"
#include <Windows.h>
#ifdef CPP11
#include <mutex>
#endif
//#include </>
#include "GC.h"
#include "scope.h"
#include "Class.h"
#include "Array.h"
#include "Function.h"
namespace lang
{
	GC::GC(scope* root)
	{
		GCtimig = 65536;
		this->NowGabekore = false;
		//this->NowAddRoot = false;
		//this->NowAddObject = false;
		count = 0;
		this->root = root;
		this->addRoot(root);
		objectCount = 0;
	}
	void GC::addObject(Object* obj)
	{
#ifdef CPP11
		std::lock_guard<GCmutex> lock(this->ObjectMutex);
#endif
		objectCount++;
		if (objectCount >= GCtimig)
		{
			this->start();
		}
		this->object[obj] = 0;
	}
	void GC::start(void)
	{
		//std::lock_guard<GCmutex> lock(this->RootMutex);
		//std::lock_guard<GCmutex> lock2(this->ObjectMutex);
		if (NowGabekore)
		{
#if _DEBUG
			if (gc_view)
				std::cout << "がべこれがブロックされました。" << std::endl;
#endif
			return;
		}
		NowGabekore = true;
		count++;
		this->object[NULLOBJECT] = count;//定数がGCに回収される
		this->object[TRUEOBJECT] = count;//定数がGCに回収される
		this->object[FALSEOBJECT] = count;//定数がGCに回収される
#if _DEBUG
		if (gc_view)
			std::cout << "がべこれ開始" << std::endl;
#endif
		if (objectCount > 160)
			std::cout << ""
			;
		// this->search(this->root);
		FOREACH(root, this->roots)//for(auto root : this->roots)
			//{
			this->search(root.first);
	}
	FOREACH(o, this->constroot)//for(auto o : this->constroot)
		//{
		this->search(o);
}
FOREACH(obj, this->object)//for(auto obj : this->object)
//{
if (obj.second != count)
{
	obj.second = count;
	erased.push_back(obj.first);
}
		}
		FOREACH(obj, erased)//for(auto obj : erased)
			//{
		if (this->object.find(obj) != this->object.end())
		{
			if (obj is _Function && ((langFunction)obj)->working)
				//マルチスレッドなどでどこからも参照されていないが動いている状態の時は開放しない
				;
			else
				object.erase(obj);
			delete obj;
		}
		}
		erased.clear();
#if _DEBUG
		if (gc_view)
			std::cout << "がべこれ終了" << std::endl;
#endif
		objectCount = object.size();
		if (objectCount <= GCtimig / 4) GCtimig /= 2;
		if (objectCount >= GCtimig / 2) GCtimig *= 2;
		//_TCHAR buf[256];
		//_tprintf_s(buf,"count:%d\tmax%d\n",objectCount,GCtimig);
		char buf[256];
#ifdef _WIN32
		sprintf_s(buf, "count:%d\tmax%d\n", objectCount, GCtimig);
#else
		sprintf(buf, "count:%d\tmax%d\n", objectCount, GCtimig);
#endif
		OutputDebugStringA(buf);
		NowGabekore = false;
	}
	void GC::search(scope* root)
	{
		//        for(auto obj : root->variable._variable)
		FOREACH(obj, root->variable._variable)//        {
		if (obj.second->type->TypeEnum == PreType::_ClassObject)
		{
			obj.second->type->TypeEnum = PreType::_ClassObject;
		}
		if (this->object.find(obj.second) != this->object.end() && this->object[obj.second] != count)
		{
			switch (obj.second->type->TypeEnum)
			{
				case lang::_Class://削除しない
					//再帰する必要が
					FOREACH(i, *((langClass)obj.second)->member)//                    foreach_(var_ i in_ *((langClass)obj.second)->member)
						//                    {
					if (this->object[i.second] != count)
					{
						this->search(i.second);//
					}
			}
			if (((langClass)obj.second)->thisscope)
				FOREACH(i, ((langClass)obj.second)->thisscope->variable._variable)// foreach_(var_ i in_ ((langClass)obj.second)->thisscope->variable._variable)
				//{
			if (this->object[i.second] != count)
			{
				this->search(i.second);//
			}
		}
		break;
                case lang::_Array:
					//                    foreach_(var_ i in_ ((langArray)obj.second)->ary)
					FOREACH(i, ((langArray)obj.second)->ary)//                    {
					if (this->object[i] != count)
					{
						this->search(i);//
					}
	}

	break;
				}
				this->object[obj.second] = count;//++;//this->object[obj] = 0;4
			}
		}
	}
	void GC::search(langObject object)
	{
		if (object == nullptr) return;
		switch (object->type->TypeEnum)
		{
			case lang::_Object:
			case lang::_Int:
			case lang::_String:
			case lang::_Char:
			case lang::_Double:
			case lang::_Function:
			default:
				this->object[object] = count;
				break;
			case lang::_Class:
				FOREACH(i, *((langClass)object)->member)//            foreach_(var_ i in_ *((langClass)object)->member)
					//            {
				if (this->object[i.second] != count)
				{
					this->search(i.second);//
				}
			}
			if (((langClass)object)->base)
			{
				this->search(((langClass)object)->base);
			}
			break;
        case lang::_Array:
			this->object[object] = count;
			FOREACH(i, ((langArray)object)->ary)//            foreach_(var_ i in_ ((langArray)object)->ary)
				//            {
			if (this->object[i] != count)
			{
				this->search(i);//
			}
	}
	break;
        case lang::_ClassObject:
			this->object[object] = count;
			if (((langClass)object)->base)
			{
				this->search(((langClass)object)->base);
			}
			break;
		}
	}
	void GC::search(langClassObject object)
	{
		if (this->object[object] == count) return;//mark
		//foreach_(var_ i in_ *object->member)
		//{
		FOREACH(i, *object->member)
		if (this->object[i.second] == count) continue;//mark
		//for(auto root : this->roots)
		//{
		FOREACH(root, this->roots)
			this->search(i.second);
	}
		}
	}
	//GCから削除してメモリから解放する
	void GC::free_(langObject object)
	{
#ifdef CPP11
		std::lock_guard<GCmutex> lock(this->RootMutex);
#endif
		if (this->object.find(object) != this->object.end())this->object.erase(object);
		delete object;
	}
	//GCから削除
	//責任
	void GC::uncontroll(langObject object)
	{
#ifdef CPP11
		std::lock_guard<GCmutex> lock(this->RootMutex);
#endif
		if (this->object.find(object) != this->object.end()) this->object.erase(object);
	}
	GC::~GC(void)
	{
	}
	void GC::addRoot(scope* root)
	{
#ifdef CPP11
		std::lock_guard<GCmutex> lock(this->RootMutex);
#endif
		this->roots[root] = 0;
	}
	//既に無いのに削除したらfalse
	bool GC::removeRoot(scope* root)
	{
#ifdef CPP11
		std::lock_guard<GCmutex> lock(this->RootMutex);
#endif
		if (this->roots.find(root) != this->roots.end())
		{
			this->roots.erase(root);
			//this->RootMutex.unlock();
			return true;
		}
		return false;
	}
}
