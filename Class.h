#pragma once
#include "lang.h"
#include "Object.h"
#include "variable.h"
namespace lang
{
    class scope;
    typedef std::vector<std::pair<std::string,langObject>> membertype_;
    typedef membertype_* membertype;
    class Class : public Object
    {
    protected:
        Class(Class* clas);
    public:
        membertype member;
        langFunction finalize;
        Class(std::string name,int index,membertype member,scope* scope,membertype staticmember);
        virtual ~Class(void);
        //名前
        std::string name;
        lang::scope* scope;
        int index;
        virtual std::string toString();
        //スコープ
		lang::scope* thisscope;
		Class* base;
		virtual langObject getMember(std::string& name);
		virtual langObject setMember(std::string& name, langObject obj);
    };
    class ClassObject : public Class
    {
    public:
        Class* staticClass;
        virtual std::string ClassObject::toString(void);
        ClassObject(Class* type);
        ~ClassObject(void);
    };
}
