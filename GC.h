#pragma once
#include "Object.h"
#include <vector>
//#include "scope.h"
namespace lang
{
class scope;
class GC
{
    private:
    int count;
    std::vector<Object*> erased;
public:
    void addRoot(scope* root);
    void removeRoot(scope* root);
    bool NowGabekore;
    scope* root;
    std::map<Object*,int> object;
    std::map<scope*,int> roots;
    void addObject(Object* obj);
    void start(void);
    void search(scope* root);
    void GC::search(langObject object);
    void GC::search(langClassObject object);
    void GC::free(langObject object);
    GC(scope* root);
    ~GC(void);
};
typedef GC ‚ª‚×‚±‚ê;
#pragma once
}
