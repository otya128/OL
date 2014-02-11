#include "stdafx.h"
#include "GC.h"
#include "scope.h"
#include "Class.h"
namespace lang
{
GC::GC(scope* root)
{
    this->NowGabekore = false;
    count = 0;
    this->root = root;
    this->addRoot(root);
}
void GC::addObject(Object* obj)
{
    this->object[obj] = 0;
}
void GC::start(void)
{
    if(NowGabekore)
    {
        std::cout<<"がべこれがブロックされました。"<<std::endl;
        return;
    }
    NowGabekore = true;
    count++;
    std::cout<<"がべこれ開始"<<std::endl;
   // this->search(this->root);
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
        if(this->object.find(obj) != this->object.end())object.erase(obj);
        delete obj;
    }
    erased.clear();
    std::cout<<"がべこれ終了"<<std::endl;
    NowGabekore = false;
}
void GC::search(scope* root)
{
    for(auto obj : root->variable._variable)
    {
        if(obj.second->type->TypeEnum == PreType::_ClassObject)
        {
            
        }
        if(this->object.find(obj.second) != this->object.end())
        {
            /*switch (obj.second->type->TypeEnum)
            {
            case lang::_ClassObject:
                this->search((langClassObject)obj.second);
                break;
            }*/
            this->object[obj.second] = count;//++;//this->object[obj] = 0;4
        }
    }
}
void GC::search(langObject object)
{
    
}
void GC::search(langClassObject object)
{
    if(this->object[object] == count) return;//mark
    foreach_(var_ i in_ *object->member)
    {
        if(this->object[i.second] == count) continue;//mark
        for(auto root : this->roots)
        {
            this->search(i.second);
        }
    }
}
GC::~GC(void)
{
}
    void GC::addRoot(scope* root)
    {
        this->roots[root] = 0;
    }
    void GC::removeRoot(scope* root)
    {
        if(this->roots.find(root) != this->roots.end())
        {
            this->roots.erase(root);
        }
    }
}
