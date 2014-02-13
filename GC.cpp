#include "stdafx.h"
#include "GC.h"
#include "scope.h"
#include "Class.h"
namespace lang
{
    GC::GC(scope* root)
    {
        GCtimig = 128;
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
#if _DEBUG
            if(gc_view)
                std::cout<<"���ׂ��ꂪ�u���b�N����܂����B"<<std::endl;
#endif
            return;
        }
        NowGabekore = true;
        count++;
        this->object[NULLOBJECT] = count;//�萔��GC�ɉ�������
#if _DEBUG
        if(gc_view)
            std::cout<<"���ׂ���J�n"<<std::endl;
#endif
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
            if(this->object.find(obj) != this->object.end())
            {
                object.erase(obj);
                delete obj;
            }
        }
        erased.clear();
#if _DEBUG
        if(gc_view)
            std::cout<<"���ׂ���I��"<<std::endl;
#endif
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
                switch (obj.second->type->TypeEnum)
                {
                case lang::_Class://�폜���Ȃ�
                    //�ċA����K�v��
                    foreach_(var_ i in_ *((langClass)obj.second)->member)
                    {
                        if(this->object[i.second] != count)
                        {
                            this->search(i.second);//
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
        switch (object->type->TypeEnum)
        {
        case lang::_Object:
        case lang::_Int:
        case lang::_String:
        case lang::_Char:
        case lang::_Double:
        case lang::_Array:
        case lang::_Class:
        case lang::_Function:
        default:
            this->object[object] = count;
            break;
        case lang::_ClassObject:
            this->object[object] = count;
            break;
        }
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
    //GC����폜���ă���������������
    void GC::free_(langObject object)
    {
        if(this->object.find(object) != this->object.end())this->object.erase(object);
        delete object;
    }
    //GC����폜
    //�ӔC
    void GC::uncontroll(langObject object)
    {
        if(this->object.find(object) != this->object.end()) this->object.erase(object);
    }
    GC::~GC(void)
    {
    }
    void GC::addRoot(scope* root)
    {
        this->roots[root] = 0;
    }
    //���ɖ����̂ɍ폜������false
    bool GC::removeRoot(scope* root)
    {
        if(this->roots.find(root) != this->roots.end())
        {
            this->roots.erase(root);
            return true;
        }
        return false;
    }
}
