#include "stdafx.h"
#include "GC.h"
#include "scope.h"
namespace lang
{
GC::GC(scope* root)
{
    count = 0;
    this->root = root;
}
void GC::addObject(Object* obj)
{
    this->object[obj] = 0;
}
void GC::start(void)
{
    count++;
    std::cout<<"‚ª‚×‚±‚êŠJŽn"<<std::endl;
    this->search(this->root);
    for(auto root : this->roots)
    {
        this->search(root.first);
    }
    for(auto obj : this->object)
    {
        if(obj.second != count)
        {
            obj.second = count;
            erased.push_back(obj.first);
        }
    }
    for(auto obj : erased)
    {
        object.erase(obj);
        delete obj;
    }
    erased.clear();
    std::cout<<"‚ª‚×‚±‚êI—¹"<<std::endl;
}
void GC::search(scope* root)
{
    for(auto obj : root->variable._variable)
    {
        if(obj.second->type->TypeEnum == PreType::_Function)
        {
            
        }
        if(this->object.find(obj.second) != this->object.end())
        {
            this->object[obj.second] = count;//++;//this->object[obj] = 0;
        }
    }
}
GC::~GC(void)
{
}
}
