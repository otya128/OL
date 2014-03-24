#ifndef __LANG_GC_H__
#define __LANG_GC_H__//#pragma once
#include "Object.h"

#ifdef CPP11
#include <mutex>
#endif
#include <vector>
//#include "scope.h"
namespace lang
{
#ifdef CPP11
    typedef std::recursive_mutex GCmutex;
#endif
    class scope;
    class GC
    {
    private:
        int count;
        std::vector<Object*> erased;
        //bool NowAddObject;
        //bool NowAddRoot;
#ifdef CPP11
        GCmutex ObjectMutex;
        GCmutex RootMutex;
#endif
    public:
        std::vector<langObject> constroot;
        size_t objectCount;
        int GCtimig;
        void addRoot(scope* root);
        bool removeRoot(scope* root);
        bool NowGabekore;
        scope* root;
        std::map<Object*,int> object;
        std::map<scope*,int> roots;
        void addObject(Object* obj);
        void start(void);
        void search(scope* root);
        void search(langObject object);
        void search(langClassObject object);
        void free_(langObject object);
        void uncontroll(langObject object);
        GC(scope* root);
        ~GC(void);
    };
    typedef GC gabekore;
//#pragma once
}
#endif 