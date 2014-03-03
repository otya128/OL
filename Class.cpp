#include "stdafx.h"
#include "Class.h"
#include "scope.h"
#include "lang.h"
#include "Function.h"
#include "langException.h"
namespace lang
{
    Class::Class(std::string name,int index,membertype member,lang::scope* scope)
    {
        this->scope = scope;
        this->scope->refinc();
        this->type = new Type(PreType::_Class, (char*)"class");
        this->name = name;
        this->index = index;
        this->member = member;
        this->finalize = nullptr;
        for(auto i : *member)
        {
            if(i.first == "finalize")
            {
                if(i.second is _Function)
                {
                    this->finalize = (langFunction)i.second;
                    break;
                }
            }
        }
    }
    Class::Class(Class* clas)
    {
        this->type = new Type(PreType::_Class, (char*)"class");
        this->name  = clas->name;
        this->index = clas->index;
        this->member= clas->member;
        this->scope = clas->scope;
        this->finalize = nullptr;
    }
    std::string Class::toString()
    {
        return "class:" + this->name;
    }
    Class::~Class(void)
    {
        if(this->type->TypeEnum == PreType::_Class)
        {delete this->member;
        this->scope->refdec();}//delete this->name;
#if _DEBUG
        if(lang::gc_view)std::cout<<"remove"<<this<<this->type->name<<std::endl;
#endif
        //delete this->type->name;
    }
    ClassObject::ClassObject(Class* type) : Class(type)//type->name,type->index,type->member,type->scope)
    {
        thisscope = new lang::scope(type->scope->parsers, type->scope,this);
        this->scope = type->scope;
        this->thisscope->refinc();
        this->type->TypeEnum = _ClassObject;
        foreach_(var_ i in_ *this->member)//C# style foreach
        {
            if(i.second->type->TypeEnum == _Function)
            {
                auto buf = new Function((langFunction)i.second,this->thisscope);
                buf->scope = thisscope;
                this->thisscope->variable.add(i.first, buf);
            }
            else
                this->thisscope->variable.add(i.first, i.second);
        }
        if(type->finalize != nullptr)
        {
            this->finalize = new Function((langFunction)type->finalize,this->thisscope);
            this->finalize->scope = thisscope;
            this->thisscope->variable.add("finalize", this->finalize);
            lang::gc->uncontroll(this->finalize);
        }

    }

    std::string ClassObject::toString(void)
    {
        if(this->thisscope->variable["ToString"] is _Function)
        {
            auto arg = std::vector<langObject>();//���ʂɃ��[�J���ϐ��̃|�C���^���ŗǂ�����
            auto result = ((langFunction)this->thisscope->variable["ToString"])->call(&arg);
            return result->toString();
        /*
            auto arg = new std::vector<langObject>;//��O�����ŋt�ɏ璷�ɂȂ��
            try
            {
                auto result = ((langFunction)this->thisscope->variable["ToString"])->call(arg);
                delete arg;
                return result->toString();
            }
            catch(...)
            {
                delete arg;
                throw;
            }*/
        }
        return Class::toString();
    }
    ClassObject::~ClassObject(void)
    {
        this->thisscope->refdec();
        if(this->finalize)//�����I�����Ă���t�@�C�i���C�U�͌Ă΂�Ȃ�
        {
            if(lang::running)
            {
                std::vector<langObject> arg;
                this->finalize->call(&arg);
            }
            delete this->finalize;
        }
        //this->scope->refdec();//scope����Ȃ���thisscope�ł�
    }
}
