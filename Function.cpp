#include "stdafx.h"
#include "Function.h"
#include <sstream>
#include "langException.h"
namespace lang
{

    Function::Function(std::string name,std::vector<std::string>* argList,lang::scope* scope,int index)
    {
#ifdef CPP11
        this->thread = nullptr;
#endif
        this->type = new Type(PreType::_Function);
        this->name = name;
        this->argList = argList;
        this->thisscope = nullptr;
        this->scope = scope;//std::make_shared<lang::scope>(*scope);
        if(this->scope != nullptr)scope->refinc();
        this->index = index;
    }
    Function::Function(Function* f,lang::scope* this_scope)
    {
#ifdef CPP11
        this->thread = nullptr;
#endif
        //if(f is _Function){}
        this->type = new Type(f->type->TypeEnum,(char*)f->type->name);
        this->name = f->name; 
        this->argList = new std::vector<std::string>(*f->argList);
        this->scope = f->scope;
        this->index = f->index;
        this->thisscope = this_scope;
        if(this->thisscope != nullptr)this_scope->refinc();
        if(this->scope != nullptr)this->scope->refinc();
        this->ptr = nullptr;
    }
    Function::~Function(void)
    {
        //if(thread)thread->detach();
#ifdef CPP11
        delete thread;
#endif

        if(this->scope != nullptr)this->scope->refdec();
        if(this->thisscope != nullptr)this->thisscope->refdec();
        //this->scope = nullptr;
        //this->ptr = new int(1);

        delete this->argList;
        //if(name != nullptr)
#if _DEBUG
        if(gc_view)
        {
            std::cout<<"‚ª‚×‚±‚ê’†..."<< name << this <<std::endl;
            //delete this->name;
        }
#endif
        // else std::cout<<"‚ª‚×‚±‚ê’†...null"<<this<<std::endl;
    }
    std::string Function::toString(void)
    {
        return "function:" + name;
    }
    langObject Function::call(std::vector<langObject>* argList)
    {
        working = true;
        auto sc =new lang::scope(this->scope->parsers,this->scope/*.get()*/,this->scope->_this);
        sc->type = en::scopeType::_function;
        sc->startIndex = this->index + 1;//+1‚µ‚ñ‚ ‚¢‚Æreturn‚ª–³‚¢ŠÖ”‚Åreturn‚³‚ê‚È‚­‚È‚é
        if(this->argList->size()!=argList->size())
            throw langRuntimeException("ˆø”‚Ì”‚ªˆá‚¤");
        for(int i=0;i<this->argList->size();i++)
        {
            sc->variable.add((*this->argList)[i],(*argList)[i]);
        }
        auto buf = sc->run();
        sc->del();
        working = false;
        return /*std::shared_ptr<Object>*/( buf);
    }
    langObject Function::ctorcall(std::vector<langObject>* argList)
    {
        auto sc =new lang::scope(this->scope->parsers,this->scope,this->scope->_this);
        sc->type = en::scopeType::ctor;
        sc->startIndex = this->index + 1;//+1‚µ‚ñ‚ ‚¢‚Æreturn‚ª–³‚¢ŠÖ”‚Åreturn‚³‚ê‚È‚­‚È‚é
        if(this->argList->size()!=argList->size()) throw langRuntimeException("ˆø”‚Ì”‚ªˆá‚¤");
        for(int i=0;i<this->argList->size();i++) sc->variable.add((*this->argList)[i],(*argList)[i]);
        auto buf = sc->run();
        sc->del();
        return buf;
    }
    langFunction scope::anonymousFunction(int& index)
    {
        std::stringstream str;
        str<<"anonymousFunction"<<index;
        //str=;
		std::string name = str.str();
        int funcRead=2;
        std::vector<std::string>* argList = new std::vector<std::string>();

        langFunction func = /*std::make_shared<Function>*/new Function(name,nullptr,this,index+1);
        for(int i=index+1;i<this->parsers.size();i++)
        {
            auto token = this->parsers[i];
            if(!funcRead)break;
            switch (funcRead)
            {
            case 2://(
				//–¼‘O‚ ‚è“½–¼ŠÖ”
				if (token->pEnum == parserEnum::identifier)
				{
					func->name = *token->name;
					continue;
				}
                if(token->pEnum == parserEnum::leftparent)funcRead++;else funcRead = 0;
				if (token->pEnum == parserEnum::rightparent)funcRead = 6;
				//ˆø”È—ª“½–¼ŠÖ”
				if (token->pEnum == parserEnum::blockstart) funcRead = 6;
                break;
            case 3://type 
                if(token->pEnum == parserEnum::identifier)funcRead++;else funcRead = 0;
                if(token->pEnum == parserEnum::rightparent)funcRead = 6;
                break;
            case 4://name
                if(token->pEnum == parserEnum::identifier)
                {
                    argList->push_back(*token->name);
                    funcRead++;
                }
                else
				if (token->pEnum == parserEnum::comma || token->pEnum == parserEnum::rightparent)
				{
					funcRead--;
					argList->push_back(*((this->parsers[i - 1])->name));
					if (token->pEnum == parserEnum::rightparent)funcRead = 6;
				}
				else
					funcRead = 0;
                break;
            case 5://, or )
                if(token->pEnum == parserEnum::comma)funcRead-=2;
                else if(token->pEnum == parserEnum::rightparent)funcRead++; 
                else funcRead = 0;
                break;
            case 6://{
                funcRead = 0;
                func->argList = argList;
                func->index = i;
                //func = new Function(name, argList, this,i);
                //this->runner->variable.add(*funcName,std::make_shared<langFunction>(*funcName,argList,this->runner));
                break;
            }
        }
        if(func->argList == nullptr)throw "syntax error";
        this->refinc();
        index = this->blockSkip(func->index);
        return /*std::shared_ptr<Function>*/(func);
    }
    langObject Object_ToString(langObject obj, std::vector<langObject> argList)
    {
        return newString(&obj->toString());
    }
    langObject String_Substring(langObject obj, std::vector<langObject> argList)
    {
        if(argList.size() == 1 && obj is _String)
        {
            auto bug = (langString)obj;
            return newString(&bug->getString()->substr(Int::toInt(argList[0])));
        }
        else
            if(argList.size() == 2 && obj is _String)
            {
                auto bug = (langString)obj;
                return newString(&bug->getString()->substr(Int::toInt(argList[0]), Int::toInt(argList[1])));
            }
            throw lang::langRuntimeException("ˆø”‚Ì”‚ªˆê’v‚µ‚Ü‚¹‚ñ[String.Substring(,)]");
    }
    typedef langObject (*BuiltFunc)(std::vector<langObject>);
    std::map<std::string,BuiltFunc>* BuiltFunction = new std::map<std::string,BuiltFunc>;
    langObject (*FuncTable[])(langObject, std::vector<langObject>) =
    {
        &Object_ToString,
        &String_Substring,
    };
    SpecialFunction::~SpecialFunction(void)
    {
    }
}
