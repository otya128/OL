#pragma once
#include "Object.h"
#include <mutex>
#include <vector>
//#include "scope.h"
namespace lang
{
    typedef std::recursive_mutex GCmutex;
    class scope;
    class GC
    {
    private:
        int count;
        std::vector<Object*> erased;
        //bool NowAddObject;
        //bool NowAddRoot;
        GCmutex ObjectMutex;
        GCmutex RootMutex;
    public:
        std::vector<langObject> constroot;
        int objectCount;
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
        void GC::search(langObject object);
        void GC::search(langClassObject object);
        void GC::free_(langObject object);
        void GC::uncontroll(langObject object);
        GC(scope* root);
        ~GC(void);
    };
    typedef GC ‚ª‚×‚±‚ê;
#pragma once
}
