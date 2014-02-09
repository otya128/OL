#pragma once
#include "Object.h"
#include <vector>
//#include "scope.h"
#define GCENABLE 1
namespace lang
{
class scope;
class GC
{
    private:
    int count;
    std::vector<Object*> erased;
public:
    scope* root;
    std::map<Object*,int> object;
    std::map<scope*,int> roots;
    void addObject(Object* obj);
    void start(void);
    void search(scope* root);
    GC(scope* root);
    ~GC(void);
};
typedef GC ‚ª‚×‚±‚ê;
#pragma once
}
