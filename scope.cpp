#pragma once
#include "stdafx.h"
#include "scope.h"
//#include "variable.h"
#include <tchar.h>
#include <map>
#include <memory>
#include <vector>
#ifdef CPP11
#include <thread>
#include <mutex>
#endif
#include <sstream>
#include <iosfwd>
#include "Function.h"
#include "GC.h"
#include "langException.h"
#include "lang.h"
#include "Class.h"
#include "Array.h"
#include "OLWindow.h"
namespace lang
{//VisualStadio
    typedef langObject (*BuiltFunc)(std::vector<langObject>);
    extern std::map<std::string,BuiltFunc>* BuiltFunction;
    namespace lib
    {
#ifdef CPP11
        std::mutex printm;
#endif
        ///thread safe?
        langObject print(std::vector<langObject> arg)
        {
            {
#ifdef CPP11
                std::lock_guard<std::mutex> lock(printm);//lock
#endif
                //if(locktst)while(locktst); locktst = true;
#if CPP11
                for (auto var : arg)
                {
#else
                for( auto it = arg.begin(); it != arg.end(); ++it )
                {
                    auto var = *it;
#endif
                    if(var != nullptr)std::cout<<var->toString(); else std::cout<< "nullptr";
                }
                //locktst = false;
            }
            return NULLOBJECT;
        }
        langObject getline(std::vector<langObject> arg)
        {
            std::string str;
            std::getline(std::cin,str);
            return newString(&str);
        }
        langObject GC(std::vector<langObject> arg)
        {
#if _DEBUG
            if(gc_view)std::cout<<"明示的ながべこれ呼び出し"<<std::endl;
#endif
            gc->start();
            return NULLOBJECT;
        }
        langObject free(std::vector<langObject> arg)
        {
#if CPP11
            foreach_(var_ i in_ arg)
            {
#else
            for( auto it = arg.begin(); it != arg.end(); ++it )
            {
                auto i = *it;
#endif
                gc->free_(i);
            }
            return NULLOBJECT;
        }
        langObject uncontrollGC(std::vector<langObject> arg)
        {
#if CPP11
            foreach_(var_ i in_ arg)
            {
#else
            for( auto it = arg.begin(); it != arg.end(); ++it )
            {
                auto i = *it;
#endif
                gc->uncontroll(i);
            }
            return NULLOBJECT;
        }
        langObject VarPtr(std::vector<langObject> arg)
        {
            return newInt((int)arg[0]->getPointer());
        }
        langObject WriteMemory(std::vector<langObject> arg)
        {
            auto ptr = (int*)Int::toInt(arg[0]);
            *ptr = Int::toInt(arg[1]);
            return NULLOBJECT;
        }
        langObject ReadMemoryInt(std::vector<langObject> arg)
        {
            auto ptr = (int*)Int::toInt(arg[0]);
            return newInt(*ptr);
        }
        langObject sqrt(std::vector<langObject> arg)
        {
            return /*std::make_shared<Int>*/newInt((int)std::sqrt((double)lang::Int::toInt(arg[0])));
        }
#ifdef CPP11
        langObject thread(std::vector<langObject> arg)
        {
            langFunction threadFunc = new Function((langFunction)arg[0],((langFunction)arg[0])->thisscope);
            std::thread* thd = new std::thread([threadFunc]
            {
                std::vector<langObject> rarg;
                threadFunc->call(&rarg);
                threadFunc->thread->detach();
                lang::gc->free_(threadFunc);
            });
            threadFunc->thread = thd;
            return NULLOBJECT;
        }
        langObject thread_join(std::vector<langObject> arg)
        {
            langFunction threadFunc = new Function((langFunction)arg[0],((langFunction)arg[0])->thisscope);
            bool thread_end = false;
            std::thread* thd = new std::thread([threadFunc,&thread_end]
            {
                std::vector<langObject> rarg;
                threadFunc->call(&rarg);
                threadFunc->thread->detach();
                lang::gc->free_(threadFunc);
                thread_end = true;
            });
            threadFunc->thread = thd;
            try
            {
                if(!thread_end)thd->join();
            }
            catch(...)
            {
            }
            return NULLOBJECT;
        }
        langObject threadid(std::vector<langObject> arg)
        {
            std::stringstream a;
            a<<std::this_thread::get_id();
            return newString(&a.str());
        }
#endif
        langObject Fopen(std::vector<langObject> arg)
        {
            FILE* fp;
            fopen_s(&fp,arg[0]->toString().c_str(),arg[1]->toString().c_str());
            return newObject((void*)fp);
        }
        langObject Fclose(std::vector<langObject> arg)
        {
            return newObject((void*)fclose((FILE*)arg[0]->getPointer()));
        }
        langObject Fwrite(std::vector<langObject> arg)
        {
            auto str = arg[0]->toString();
            return newObject((void*)fwrite(str.data(),sizeof(char),str.size(),(FILE*)arg[1]->getPointer()/*(FILE*)(size_t)Int::toInt(arg[1])*/));
        }
        langObject getstdin(std::vector<langObject> arg)
        {
            return newObject((void*)stdin);
        }
        langObject getstdout(std::vector<langObject> arg)
        {
            return newObject((void*)stdout);
        }
        langObject getstderr(std::vector<langObject> arg)
        {
            return newObject((void*)stderr);
        }
        langObject olruntime_gc_objectcount(std::vector<langObject> arg)
        {
            return newInt(lang::gc->objectCount);
        }
        langObject olruntime_gc_gctiming(std::vector<langObject> arg)
        {
            return newInt(lang::gc->GCtimig);
        }
        langObject olruntime_gc_run(std::vector<langObject> arg)
        {
            return newInt(lang::gc->NowGabekore);
        }
        template<class T>
        class ObjectWindow : public Object
        {
        public:
            ObjectWindow()
            {
                this->ptr = &win;
            }
            std::map<std::string,langFunction> Event;
            T win; 
        };
        typedef ObjectWindow<OLWindow> ObjectWin;
        typedef ObjectWindow<Button> ObjectBtn;
        typedef ObjectWindow<Label> ObjectLabel;
        typedef ObjectWindow<TextBox> ObjectTextBox;
        typedef ObjectWindow<CheckBox> ObjectCheckBox;
        langObject window_create(std::vector<langObject> arg)
        {
            ObjectWin* a = new ObjectWin();
            std::string str2 = arg[0]->toString();
            int len = str2.length();
            TCHAR* buf = new TCHAR[len * 2];
            buf[MultiByteToWideChar(CP_OEMCP,MB_PRECOMPOSED,str2.c_str(),len,buf,len * 2)] = _T('\0');
            a->win.Copy(OLWindow(buf,Int::toInt(arg[1]),Int::toInt(arg[2])));
            delete buf;
            //OLWindow::windowmap[a->win.hWnd] = &a->win;
            a->win.Tag = a;
            a->win.SetFont(_T("MS UI Gothic"), 9);
            //
            return a;
        }
        langObject window_show(std::vector<langObject> arg)
        {
            ObjectWin* a = dynamic_cast<ObjectWin*>(arg[0]);
            a->win.Show();
            return a;
        }
        langObject window_gettext(std::vector<langObject> arg)
        {
            ObjectWin* a = static_cast<ObjectWin*>(arg[0]);
            TCHAR* text = a->win.GetText();
            langString result;
            int len = _tcsclen(text);
            char *mbs = new char[len * 2 + 2];
            mbs[WideCharToMultiByte(CP_ACP,0,text,-1,mbs,len * 2 + 1,NULL,NULL)] = '\0';
            result = newString(&std::string(mbs));
            delete [] mbs;
            delete [] text;
            return result;
        }
        langObject window_settext(std::vector<langObject> arg)
        {
            ObjectWin* a = static_cast<ObjectWin*>(arg[0]);
            std::string str2 = arg[1]->toString();
            int len = str2.length();
            TCHAR* buf = new TCHAR[len * 2 + 1];
            buf[MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,str2.c_str(),len,buf,len * 2)] = _T('\0');
            a->win.SetText(buf);
            delete buf;
            return a;
        }/*
         class ObjectBtn : public ObjectWin
         {
         public:
         ObjectBtn()
         {
         this->ptr = &win;
         }
         Button win;
         };*/
        langObject button_create(std::vector<langObject> arg)
        {
            ObjectWin* parent = dynamic_cast<ObjectWin*>(arg[0]);
            ObjectBtn* a = new ObjectBtn();
            std::string str2 = arg[1]->toString();
            int len = str2.length();
            TCHAR* buf = new TCHAR[len * 2];
            buf[MultiByteToWideChar(CP_OEMCP,MB_PRECOMPOSED,str2.c_str(),len,buf,len * 2)] = _T('\0');
            a->win.Copy(Button(parent->win,buf,Int::toInt(arg[2]),Int::toInt(arg[3]),Int::toInt(arg[4]),Int::toInt(arg[5])));
            delete buf;
            //OLWindow::windowmap[a->win.hWnd] = &a->win;
            a->win.Tag = a;
            a->win.SetFont(_T("MS UI Gothic"), 9);
            return a;
        }
        langObject label_create(std::vector<langObject> arg)
        {
            ObjectWin* parent = dynamic_cast<ObjectWin*>(arg[0]);
            ObjectLabel* a = new ObjectLabel();
            std::string str2 = arg[1]->toString();
            int len = str2.length();
            TCHAR* buf = new TCHAR[len * 2];
            buf[MultiByteToWideChar(CP_OEMCP,MB_PRECOMPOSED,str2.c_str(),len,buf,len * 2)] = _T('\0');
            a->win = Label(parent->win,buf,Int::toInt(arg[2]),Int::toInt(arg[3]),Int::toInt(arg[4]),Int::toInt(arg[5]));
            delete buf;
            //OLWindow::windowmap[a->win.hWnd] = &a->win;
            a->win.Tag = a;
            a->win.SetFont(_T("MS UI Gothic"), 9);
            return a;
        }
        langObject textbox_create(std::vector<langObject> arg)
        {
            ObjectWin* parent = dynamic_cast<ObjectWin*>(arg[0]);
            ObjectTextBox* a = new ObjectTextBox();
            std::string str2 = arg[1]->toString();
            int len = str2.length();
            TCHAR* buf = new TCHAR[len * 2 + 1];
            buf[MultiByteToWideChar(CP_OEMCP,MB_PRECOMPOSED,str2.c_str(),len,buf,len * 2)] = _T('\0');
            a->win.Copy(TextBox(parent->win,buf,Int::toInt(arg[2]),Int::toInt(arg[3]),Int::toInt(arg[4]),Int::toInt(arg[5]),Int::toInt(arg[6])));
            delete buf;
            //OLWindow::windowmap[a->win.hWnd] = &a->win;
            a->win.Tag = a;
            a->win.SetFont(_T("MS UI Gothic"), 9);
            return a;
        }
        langObject textbox_setreadonly(std::vector<langObject> arg)
        {
            ObjectTextBox* textbox = dynamic_cast<ObjectTextBox*>(arg[0]);
            textbox->win.SetReadOnly(Int::toInt(arg[1]));
            return NULLOBJECT;
        }
        langObject textbox_setmultiline(std::vector<langObject> arg)
        {
            ObjectTextBox* textbox = dynamic_cast<ObjectTextBox*>(arg[0]);
            textbox->win.SetMultiLine(Int::toInt(arg[1]));
            return NULLOBJECT;
        }
        langObject textbox_getreadonly(std::vector<langObject> arg)
        {
            ObjectTextBox* textbox = dynamic_cast<ObjectTextBox*>(arg[0]);
            return newInt(textbox->win.GetReadOnly());
        }
        langObject textbox_getmultiline(std::vector<langObject> arg)
        {
            ObjectTextBox* textbox = dynamic_cast<ObjectTextBox*>(arg[0]);
            return newInt(textbox->win.GetMultiLine());
        }
        langObject checkbox_create(std::vector<langObject> arg)
        {
            ObjectWin* parent = /*dynamic*/static_cast<ObjectWin*>(arg[0]);
            ObjectCheckBox* a = new ObjectCheckBox();
            std::string str2 = arg[1]->toString();
            int len = str2.length();
            TCHAR* buf = new TCHAR[len * 2];
            buf[MultiByteToWideChar(CP_OEMCP,MB_PRECOMPOSED,str2.c_str(),len,buf,len * 2)] = _T('\0');
            a->win = CheckBox(parent->win,buf,Int::toInt(arg[2]),Int::toInt(arg[3]),Int::toInt(arg[4]),Int::toInt(arg[5]));
            delete buf;
            //OLWindow::windowmap[a->win.hWnd] = &a->win;
            a->win.Tag = a;
            a->win.SetFont(_T("MS UI Gothic"), 9);
            return a;
        }
        void window_setonclick_event(eventargs<OLWindow*> v)
        {
                ObjectWin* a = static_cast<ObjectWin*>(v.Arg->Tag);
                std::vector<langObject> arg;
                arg.push_back(a);
                a->Event["OnClick"]->call(&arg);
        }
        langObject window_setonclick(std::vector<langObject> arg)
        {
            ObjectWin* a = (ObjectWin*)arg[0];//dynamic_cast<ObjectWin*>(arg[0]);
            a->Event["OnClick"] = (langFunction)arg[1];
            a->win.OnClick += window_setonclick_event/*[](eventargs<OLWindow*> v)
            {
                ObjectWin* a = static_cast<ObjectWin*>(v.Arg->Tag);
                std::vector<langObject> arg;
                arg.push_back(a);
                a->Event["OnClick"]->call(&arg);
            }*/;
            return a;
        }
        langObject window_setfont(std::vector<langObject> arg)
        {
            ObjectWin* a = (ObjectWin*)arg[0];
            std::string str2 = arg[1]->toString();
            int len = str2.length();
            TCHAR* buf = new TCHAR[len * 2];
            buf[MultiByteToWideChar(CP_OEMCP,MB_PRECOMPOSED,str2.c_str(),len,buf,len * 2)] = _T('\0');
            if(arg.size() == 2) a->win.SetFont(buf,Int::toInt(arg[1]));
            else if(arg.size() == 7) a->win.SetFont(buf, Int::toInt(arg[2]), Int::toInt(arg[3]), Int::toInt(arg[4]), Int::toInt(arg[5]), Int::toInt(arg[6]));
            return a;
        }
        langObject messagebox(std::vector<langObject> arg)
        {
            if(arg.size() == 1)
                MessageBoxA(NULL, arg[0]->toString().c_str(),"",MB_OK);
            else
                if(arg.size() == 2)
                {
                    ObjectWin* window = dynamic_cast<ObjectWin*>(arg[1]);
                    MessageBoxA(window->win.hWnd, arg[0]->toString().c_str(),"",MB_OK);
                }
                return NULLOBJECT;
        }
#ifdef CPP11
        langObject olruntime_gc_asyncgc(std::vector<langObject> arg)
        {
            std::thread* thd = new std::thread([]{lang::gc->start();});
            return NULLOBJECT;
        }
#endif
        void Add(std::string name, BuiltFunc func)
        {
            (*lang::BuiltFunction)[name] = func;
        }
        void init()
        {
            Add("print",        print);
            Add("getline",      getline);
            Add("GC",           GC);
            Add("free",         free);
            Add("uncontrollGC", uncontrollGC);
            Add("VarPtr",       VarPtr);
            Add("WriteMemory",  WriteMemory);
            Add("ReadMemoryInt",ReadMemoryInt);
            Add("sqrt",         sqrt);
#ifdef CPP11
            Add("thread",       thread);
            Add("threadid",     threadid);
            Add("thread::join", thread_join);
#endif
            Add("File::open",   Fopen);
            Add("File::close",  Fclose);
            Add("File::write",  Fwrite);
            Add("File::stdin",  getstdin);
            Add("File::stdout", getstdout);
            Add("File::stderr", getstderr);
            Add("OLRuntime::GC::ObjectCount", olruntime_gc_objectcount);
#ifdef CPP11
            Add("OLRuntime::GC::BackgroundGC", olruntime_gc_asyncgc);
#endif
            Add("OLRuntime::GUI::Window::Create", window_create);
            Add("OLRuntime::GUI::Window::Show", window_show);
            Add("OLRuntime::GUI::Button::Create", button_create);
            Add("OLRuntime::GUI::Window::SetOnClick", window_setonclick);
            Add("OLRuntime::GUI::Window::SetFont", window_setfont);
            Add("OLRuntime::GUI::Window::GetText", window_gettext);
            Add("OLRuntime::GUI::Window::SetText", window_settext);
            Add("OLRuntime::GUI::Label::Create", label_create);
            Add("OLRuntime::GUI::TextBox::Create", textbox_create);
            Add("OLRuntime::GUI::TextBox::SetReadOnly", textbox_setreadonly);
            Add("OLRuntime::GUI::TextBox::GetReadOnly", textbox_getreadonly);
            Add("OLRuntime::GUI::TextBox::SetMultiLine", textbox_setmultiline);
            Add("OLRuntime::GUI::TextBox::GetMultiLine", textbox_getmultiline);
            Add("OLRuntime::GUI::CheckBox::Create", checkbox_create);
            Add("OLRuntime::GUI::MessageBox", messagebox);
        }
    }

    langObject BuitInFunction(std::string name,std::vector<langObject> arg)
    {
        auto func = (*BuiltFunction)[name];
        if(func)
        {
            return func(arg);
        }
        else
        {
            throw_langRuntimeException("関数%sが見つかりません。", name.c_str());
        }
#if 0
        if(name=="print")
        {
            for (auto var : arg)
            {
                if(var != nullptr)std::cout<<var->toString(); else std::cout<< "null";
            }
        }
        if(name=="getline")
        {
            std::string str;
            std::getline(std::cin,str);
            return newString(&str);
        }
        if(name=="GC")
        {
#if _DEBUG
            if(gc_view)std::cout<<"明示的ながべこれ呼び出し"<<std::endl;
#endif
            gc->start();
        }
        if(name=="free")
        {
            foreach_(var_ i in_ arg)
            {
                gc->free_(i);
            }
        }
        if(name=="uncontrollGC")
        {
            foreach_(var_ i in_ arg)
            {
                gc->uncontroll(i);
            }
        }
        if(name=="VarPtr")
        {
            return newInt((int)arg[0]->getPointer());
        }
        if(name=="WriteMemory")
        {
            auto ptr = (int*)Int::toInt(arg[0]);
            *ptr = Int::toInt(arg[1]);
            return 0;
        }
        if(name == "ReadMemoryInt")
        {
            auto ptr = (int*)Int::toInt(arg[0]);
            return newInt(*ptr);
        }
        if(name=="sqrt")
        {
            return /*std::make_shared<Int>*/newInt((int)sqrt<int>(Int::toInt(arg[0])));
        }
        return 0;
#endif
    }
    void scope::refinc()
    {
        this->refcount++;
    }
    void scope::refdec()
    {
        this->refcount--;
        if(this->refcount<=0) 
        {
            if(this->variable.parentVariable != nullptr && gc->removeRoot(this))
                delete this;
        }
    }
    void scope::del()
    {
        if(this->refcount<=0) 
        {
            if(this->variable.parentVariable != nullptr && gc->removeRoot(this))
                delete this;
        }
    }
    scope::scope(std::vector<parseObj*>& v):parsers(v)
    {
        this->isClass() = nullptr;
        refcount = 0;
        //this->variable = lang::variable();
        //this->parsers = v;
        this->index = 0;
        this->startIndex = 0;
        this->status = en::returnStatus::none_;
        this->type = en::scopeType::_none_;
#if _DEBUG
        if(gc_view) 
            std::cout<<"変数スコープを作成"<<this<<std::endl;
#endif
    }
    scope::scope(std::vector<parseObj*>& v,scope* parent,langClassObject _this):parsers(v)
    {
        this->isClass() = isClass();
        gc->addRoot(this);//gc->roots[this] = 0;
        refcount = 0;
        this->variable.parentVariable = &parent->variable;//this->variable = new lang::variable(parent->variable);
        //this->parsers = v;
        this->index = 0;
        this->startIndex = 0;
        this->status = en::returnStatus::none_;
        this->type = en::scopeType::_none_;
#if _DEBUG
        if(gc_view) 
            std::cout<<"変数スコープを作成"<<this<<std::endl;
#endif
    }


    scope::~scope(void)
    {
        gc->removeRoot(this);//gc->roots.erase(this);
#if _DEBUG
        if(gc_view) 
            std::cout<<"変数スコープを廃棄"<<this<<std::endl;
#endif
    }
    int scope::parentSkip(int index)
    {
        int j=0;
        while(this->parsers.size()-1>index)
        {
            index++;
            switch (this->parsers[index]->pEnum)
            {
            case parserEnum::leftparent:
                j++;
                break;
            case parserEnum::rightparent:
                if(j==0)
                    return index;
                j--;
                break;
            }
        }
        return index;
    }
    int scope::blockSkip(int index,int j)
    {
        while(this->parsers.size()-1>index)
        {
            index++;
            switch (this->parsers[index]->pEnum)
            {
            case parserEnum::blockstart:
                j++;
                break;
            case parserEnum::blockend:
                if(j==0)
                    return index;
                j--;
                break;
            }
        }
        return index;
    }
    langObject scope::run(void)
    {
        this->index = this->startIndex;
        auto status = en::scopeStatus::none;
        /*std::shared_ptr<scope>*/scope* forscope = 0;
        int forindex[3],findex = -1;
        forindex[0]=-1;forindex[1]=-1;forindex[2]=-1;
        try
        {
            for(this->index = this->startIndex;index < parsers.size();index++)
            {
                auto j = this->parsers[index];
                switch (status)
                {
                case en::none:
                    switch (j->pEnum)
                    {
                    case _class:
                        index = this->blockSkip(index);
                        break;
                    case parserEnum::_new:
                        eval(this->parsers[this->index]->ptr, this->index);
                        break;
                    case parserEnum::identifier:
                        if(*j->name == "for")
                        {
                            status = en::_for;
                        }else if(*j->name == "if")
                        {
                            status = en::_if;
                        }
                        else if(*j->name == "return")
                        {
                            this->status = en::returnStatus::_return;
                            index++;
                            auto buf = eval(NULLOBJECT,index);

                            return /*langObject*/(buf);
                        }
                        else
                            status = en::iden;
                        break;
                    case parserEnum::blockend:
                        if(this->type == en::ctor)return this->_this;
                        return NULLOBJECT;
                    case parserEnum::blockstart:
                        auto sc = /*std::make_shared<scope>*/new scope(this->parsers,this,this->_this);
                        sc->startIndex = this->index + 1;
                        auto buf = sc->run();
                        this->index = sc->index;
                        if(sc->status==en::returnStatus::_return)
                        {
                            this->status = sc->status;
                            sc->del();
                            //delete sc;
                            return /*langObject*/(buf);
                        }
                        //delete sc;
                        sc->del();
                        break;
                    }
                    break;
                case en::iden:
                    switch (j->pEnum)
                    {
                    case parserEnum::identifier:
                        status = en::var;
                        break;
                    default:
                        int i=index-1;
                        langObject buf = eval(this->parsers[this->index-1]->ptr,i);
                        index=i;status = en::none;
                        //if(buf!=nullptr)std::cout<<"result:"<<(buf)->toString()<<std::endl;
                        break;
                    }
                    break;
                case en::var:
                    switch (j->pEnum)
                    {
                    case parserEnum::equal:
                        {
                            int i=index-1;
                            this->variable.add(*this->parsers[this->index-1]->name,NULLOBJECT);
                            //this->variable[*this->parsers[this->index-1]->name]=
                            eval(this->parsers[this->index-1]->ptr,i);
                            status = en::none;index=i;
                        }

                        break;
                    case parserEnum::leftparent:
                        index = this->parentSkip(index);
                        status = en::none;
                        break;
                    default:
                        {
                            int i=index-2;
                            langObject buf = eval(this->parsers[i]->ptr,i);
                            index=i;status = en::none;
                            //if(buf!=nullptr)std::cout<<"result:"<<(buf)->toString()<<std::endl;
                        }
                        break;
                    }
                    break;
#ifndef forの処理
                case en::_for:
                    switch (findex)
                    {
                    case 4:
                    case 3:
                        switch (j->pEnum)
                        {
                        case parserEnum::blockstart:
                            if(findex == 3)
                            {
                                forscope = /*std::make_shared<scope>*/new scope(this->parsers,this,this->_this);
                                forscope->startIndex = this->index + 1;
                                forscope->index = forindex[0];
                                forscope->eval(NULLOBJECT,forscope->index);
                            }
                            forscope->index = forindex[1];
                            if(Int::toInt(forscope->eval(NULLOBJECT,forscope->index)))
                            {
                                auto buf = forscope->run();
                                if(forscope->status == en::returnStatus::_return)
                                {
                                    this->status = forscope->status;
                                    //delete forscope;
                                    forscope->del();
                                    return buf;
                                }
                                this->index--;//this->index = forscope->index;
                                findex = 4;
                                forscope->index = forindex[2];
                                forscope->eval(NULLOBJECT,forscope->index);
                            }
                            else
                            {
                                this->index = this->blockSkip(this->index);
                                status = en::none;
                                forscope->del();
                                //delete forscope;
                            }
                            break;
                        }
                        break;
                    case 2:
                        switch (j->pEnum)
                        {
                        case semicolon:
                            forindex[2] = index + 1;
                            findex = 3;
                            break;
                        }
                        break;
                    case 1:
                        switch (j->pEnum)
                        {
                        case semicolon:
                            forindex[1] = index + 1;
                            findex = 2;
                            break;
                        }
                        break;
                    case -1:
                        if(j->pEnum==parserEnum::leftparent)
                        {
                            forindex[0] = index+1;
                            findex = 1;
                        }
                        else throw "forの後には(が必要";
                        break;
                    }
                    break;
#endif
                case en::_if:
                    langObject buf = eval(this->parsers[this->index]->ptr,this->index);
                    if(Int::toInt(buf))
                    {
                        status = en::none;
                    }
                    else
                    {
                        this->index = this->blockSkip(this->index + 1);
                        status = en::none;
                    }
                    break;
                }
            }
        }
        catch(langRuntimeException ex)
        {
            throw langRuntimeException(ex.what(),startIndex,index,this->parsers,ex.stacktrace,ex.funcstacktrace);
        }
        return NULLOBJECT;
        //auto buf = eval(this->parsers[0]->ptr,this->index);
        //if(buf!=nullptr)std::cout<<"result:"<<(buf)->toString()<<std::endl;
        //delete buf;
    }
#if 1
    const int EqualPrece = 16;
    const int PlusPrece = 6;
    const int MinusPrece = 6;
    const int GreaterPrece = 8;
    const int GreaterEqualPrece = 8;
    const int LessPrece = 8;
    const int LessEqualPrece = 8;
    const int EqualEqualPrece = 9;
    const int MultiplyPrece = 5;
    const int ModuloPrece = 5;
    const int DivisionPrece = 5;
    const int PlusEqualPrece = 16;
    const int DotEqualPrece = 1;
    const int ArrayPrece = 2;//2;
    const int IncrementPrece = 2;
    const int PointerPrece = 3;
    int Operator(parserEnum op)
    {
        switch (op)
        {
        case parserEnum::dot:
            return 1;
        case parserEnum::leftparent://return 17;
        case parserEnum::leftbracket:
            return ArrayPrece;
        case parserEnum::plusplus:
        case parserEnum::minusminus:
            return IncrementPrece;
        case parserEnum::modulo:
        case parserEnum::multiply:
        case parserEnum::division:
            return 5;
        case parserEnum::minus:
        case parserEnum::plus:
            return 6;
        case parserEnum::leftshift://<<
        case parserEnum::rightshift://>>
            return 7;
        case parserEnum::greater:
        case parserEnum::greaterequal:
        case parserEnum::less:
        case parserEnum::lessequal:
            return 8;
        case parserEnum::equalequal:
        case parserEnum::notequal:
            return 9;
        case parserEnum::and:
            return 10;
        case parserEnum::xor:
            return 11;
        case parserEnum::or:
            return 12;
        case parserEnum::andand:
            return 13;
        case parserEnum::oror:
            return 14;
        case parserEnum::equal://=    ＼
        case parserEnum::plusequal://+= ＼
        case parserEnum::minusequal://-   ＼
        case parserEnum::divisionequal:// /=＼
        case parserEnum::leftshiftequal://<<= ＼
        case parserEnum::rightshiftequal://>>= /
        case parserEnum::multiplyequal: //*=  /
        case parserEnum::moduloequal:  //%=  /
        case parserEnum::andequal:    //&=  /
        case parserEnum::xorequal:   //^=  /
        case parserEnum::orequal:   //|=  /
            return 16;
        default:
            return 0;
        }
    }
#endif
#define OP if (opera <= thisop) break
#define OP2 i = index;if(this->parsers.size()>index+1&&Operator(this->parsers[index+1]->pEnum) >= thisop) object = eval(object,i,17,evals::isbinaryoperation),index = i;
#define OP3 i=index;if(this->parsers.size()>index+1&&Operator(this->parsers[index+1]->pEnum) >= thisop||this->parsers[index+1]->pEnum==leftparent) object = eval(object,i,17,evals::isbinaryoperation),index = i;//OP2
#define OP4 if(this->parsers.size()>index+1&&Operator(this->parsers[index+1]->pEnum) >= thisop||this->parsers[index+1]->pEnum==leftparent) object = eval(object,i,17,evals::isbinaryoperation),index = i;//OP2//i = index + 2;
#define OP5 i = index;if(this->parsers.size()>index+1&&Operator(this->parsers[index+1]->pEnum) >= thisop||this->parsers[index+1]->pEnum==leftbracket) object = eval(object,i,17,evals::isbinaryoperation),index = i;//OP2//i = index + 2;
    langObject scope::eval(langObject object,int& index,int opera,evals ev)
    {
        //int index = object->index;
        int binaryoperation = index + 1;
        int i,j;
        bool isbinaryoperation = (bool)((int)ev & 1);
        if(!isbinaryoperation)
        {
            switch (this->parsers[index]->pEnum)
            {
            case parserEnum::identifier:
                /*if(this->parsers.size()>binaryoperation&&this->parsers[binaryoperation]->pEnum==leftparent)
                {
                j=index;
                i = this->parentSkip(binaryoperation);
                std::vector<langObject> arg;
                index = index + 2;
                while (index<i)
                {
                arg.push_back(eval(NULLOBJECT,index,17));
                index++;
                if(this->parsers[index]->pEnum==parserEnum::comma)index++;
                }
                if(this->variable[*this->parsers[j]->name]!=nullptr && this->variable[*this->parsers[j]->name]->type->TypeEnum == PreType::_Function)
                {
                object = static_cast<Function*>(this->variable[*this->parsers[j]->name].get())->call(&arg);
                }
                else 
                object = BuitInFunction(*this->parsers[binaryoperation - 1]->name,arg);
                //object = 
                index = i;
                binaryoperation = index + 1;

                }
                else*/
                if(*this->parsers[index]->name == "function")
                {
                    object = /*std::make_shared<Function>*/(/**/anonymousFunction(index));
                    binaryoperation = index + 1;
                }
                else
                    object = this->variable[*this->parsers[index]->name];
                break;
            case parserEnum::leftparent:
                i = this->parentSkip(index);
                object = eval(NULLOBJECT,binaryoperation,17);
                index = i;
                binaryoperation = index + 1;
                break;
            case _new:
                if(*this->parsers[binaryoperation]->name == "Array")
                {
                    binaryoperation++;
                    object = eval(NULLOBJECT,binaryoperation,17);
                    //binaryoperation+=2;
                    index = binaryoperation;
                    binaryoperation = index + 1;
                    object = newArray(Int::toInt(object));
                }
                else
                {
                    object = eval(NULLOBJECT,binaryoperation,0);
                    index = binaryoperation;
                    binaryoperation = index + 1;

                    if(object->type->TypeEnum == PreType::_Class )
                    {
                        auto buf = (Class*)object;
                        object = newClassObject(buf);
                        if(this->parsers[binaryoperation]->pEnum == parserEnum::leftparent)
                        {
                            int thisop = Operator(this->parsers[binaryoperation]->pEnum);
                            j=index;
                            i = this->parentSkip(binaryoperation);
                            std::vector<langObject> arg;
                            index = index + 2;
                            while (index<i)
                            {
                                arg.push_back(eval(NULLOBJECT,index,17));
                                index++;
                                if(this->parsers[index]->pEnum==parserEnum::comma)index++;
                            }
                            auto ctor = ((langClassObject)object)->thisscope->variable["ctor"];
                            if(ctor->type->TypeEnum == _Function)
                            {
                                try
                                {
                                    object = langObject(static_cast<Function*>(ctor)->ctorcall(&arg));
                                }
                                catch(langRuntimeException ex)
                                {
                                    throw langRuntimeException(ex.what(),ex.tokens,ex.funcstacktrace,static_cast<Function*>(ctor)->name.c_str(),ex.stacktrace);
                                }
                            }
                            index = i;
                            binaryoperation = index + 1;
                            i = index-1;
                            //OP4
                        }
                    }

                    else
                    {
                        throw lang::langRuntimeException("new はClass型でのみ有効です。");
                    }
                }
                break;
            case parserEnum::_this:
                object = _this;
                if(this->isClass())
                {

                }
                else
                    throw lang::langRuntimeException("thisは使えません。");
                index = index + 0;
                //throw lang::langRuntimeException("new はClass型でのみ有効です。");
                break;
            case parserEnum::_false:
                object = this->parsers[index]->ptr;
                break;
            case parserEnum::_null:
            case parserEnum::_true:
            case parserEnum::num:
            case parserEnum::str:
            case parserEnum::chr:
                object = this->parsers[index]->ptr;
                break;
            }
            if(this->parsers[index] != nullptr)
            {
                //delete object;
            }
        }
        if(this->parsers.size()>binaryoperation)
        {
            langObject buf;
            int i=index+2;
            int thisop = Operator(this->parsers[binaryoperation]->pEnum);
            bool isBuilt = true;
            switch (this->parsers[binaryoperation]->pEnum)
            {
            case leftparent:
                OP;
                {
                    j=index;
                    i = this->parentSkip(binaryoperation);
                    std::vector<langObject> arg;
                    index = index + 2;
                    while (index<i)
                    {
                        arg.push_back(eval(NULLOBJECT,index,17));
                        index++;
                        if(this->parsers[index]->pEnum==parserEnum::comma)index++;
                    }
                    /*if(this->variable[*this->parsers[j]->name]!=nullptr && this->variable[*this->parsers[j]->name]->type->TypeEnum == PreType::_Function)
                    {
                    object = static_cast<Function*>(this->variable[*this->parsers[j]->name].get())->call(&arg);
                    }*/
                    if(object!=nullptr && object->type->TypeEnum == _Function)
                    {
                        try
                        {
                            object = langObject(static_cast<Function*>(object/*.get()*/)->call(&arg));
                        }
                        catch(langRuntimeException ex)
                        {
                            throw langRuntimeException(ex.what(),ex.tokens,ex.funcstacktrace,static_cast<Function*>(object)->name.c_str(),ex.stacktrace);
                        }
                        isBuilt = false;
                    }
                    else 
                        object = BuitInFunction(*this->parsers[binaryoperation - 1]->name,arg);
                    //object = 
                    index = i;
                    binaryoperation = index + 1;
                }__v('ω')v__
                if(isBuilt)
                    i = index-1;
                OP4
                    //if(this->parsers.size()>index+1&& Operator(this->parsers[index+1]->pEnum) >= thisop ||this->parsers[index+1]->pEnum==leftparent)
                    // object = eval(object,i,17,true),index = i;
                    break;
            case parserEnum::plus:
                OP;
                buf = eval(object,i,thisop);
                object = (Object::plus(object,buf));
                index  = i;
                //delete buf;
                OP2
                    break;
            case parserEnum::multiply:
                OP;
                buf = eval(object,i,thisop);
                object = (Object::multiply(object,buf));
                index = i;
                OP2
                    break;
            case parserEnum::modulo:
                OP;
                buf = eval(object,i,thisop);
                object = (Object::modulo(object,buf));
                index = i;
                OP2
                    break;
            case parserEnum::greater:
                OP;
                buf = eval(object,i,thisop);
                object = (Object::greater(object,buf));
                index = i;
                OP2
                    break;
            case parserEnum::less:
                OP;
                buf = eval(object,i,thisop);
                object = (Object::less(object,buf));
                index = i;
                OP2
                    break;
            case parserEnum::greaterequal:
                OP;
                buf = eval(object,i,thisop);
                object = (Object::greaterEqual(object,buf));
                index = i;
                OP2
                    break;
            case parserEnum::lessequal:
                OP;
                buf = eval(object,i,thisop);
                object = (Object::lessEqual(object,buf));
                index = i;
                OP2//if(this->parsers.size()>index+1&&Operator(this->parsers[index+1]->pEnum) >= thisop) object = eval(object,i,17,true),index = i;
                    break;
            case parserEnum::equalequal:
                OP;
                buf = eval(object,i,thisop);
                object = (Object::equal(object,buf));
                index = i;
                OP2
                    break;
            case parserEnum::leftshift:
                OP;
                buf = eval(object,i,thisop);
                object = (Object::leftShift(object,buf));
                index = i;
                OP2
                    break;
            case parserEnum::rightshift:
                OP;
                buf = eval(object,i,thisop);
                object = (Object::rightShift(object,buf));
                index = i;
                OP2
                    break;
            case parserEnum::equal:
                OP;
                object = eval(object,i,thisop);
                this->variable.set(*this->parsers[index]->name,object);//this->variable[*this->parsers[index]->name] = object;
                index = i;
                OP2
                    break;
            case parserEnum::plusplus:
                OP;
                //buf = newInt(1);
                object = Object::inc(object);//Object::plus(object,buf);
                this->variable.set(*this->parsers[index]->name,object);
                index = i;
                OP2
                    break;
            case parserEnum::leftbracket:
                OP;
                {
                    buf = eval(object,i,17);
                    auto ob = object;
                    //object = ((langArray)object)->ary[Int::toInt(buf)];
                    //object = (Object::plus(object,buf));
                    index  = i + 1;
                    binaryoperation  = index + 1;
                    if(object is _Array)
                    {
                        //object = ((langArray)object)->ary[Int::toInt(object)];
                        if(this->parsers.size()>index + 1)
                        {
                            if(this->parsers[index + 1]->pEnum == equal)
                            {
                                //binaryoperation++;
                                auto set = (langArray)object;
                                //index++;
                                binaryoperation++;
                                int ind = Int::toInt(buf);
                                if(set->ary.size()<=ind)

                                    throw_langRuntimeException("配列の範囲外にアクセス%xのサイズは%dで、 %dにアクセスしようとしました。:VAR_DUMP%s",set,set->ary.size(),ind,set->toString().c_str())

                                    set->ary[ind] = eval(NULLOBJECT, binaryoperation);
                                object = set->ary[Int::toInt(object)];
                                //object = buf->thisscope->variable[*bufbuf->name];
                                index = binaryoperation;
                                binaryoperation++;
                            }
                        }
                    }
                    else throw langRuntimeException("[]を使えない");
                    object = ((langArray)ob)->ary[Int::toInt(buf)];
                }
                OP5
                    break;
            case parserEnum::dot:
                OP;
                if(object->type->TypeEnum == PreType::_ClassObject || object->type->TypeEnum == PreType::_Class)
                {
                    auto buf = (Class*)object;
                    if(this->parsers.size()>binaryoperation +1)
                    {
                        auto bufbuf = this->parsers[binaryoperation + 1];
                        if(bufbuf->pEnum == identifier || bufbuf->pEnum == _static)
                        {
                            if(bufbuf->pEnum == identifier)
                            {
                                object = buf->thisscope->variable[*bufbuf->name];
                                index++;
                                binaryoperation++;
                                if(this->parsers.size()>binaryoperation + 1)
                                {
                                    if(this->parsers[binaryoperation + 1]->pEnum == equal)
                                    {
                                        //binaryoperation++;
                                        buf->thisscope->variable.set(*bufbuf->name,eval(NULLOBJECT,binaryoperation));
                                        //object = buf->thisscope->variable[*bufbuf->name];
                                        index++;
                                        binaryoperation++;
                                    }
                                }
                            }
                            else
                            {
                                if(object->type->TypeEnum == PreType::_ClassObject)
                                    object = ((langClassObject)object)->staticClass;
                                //else ; //staticクラスからstaticを取ろうとしている
                                index++;
                                binaryoperation++;
                            }
                        }
                    }
                    index++;
                    binaryoperation++;
                }
                else
                {
                    if(this->parsers.size() > binaryoperation +1)
                    {
                        auto bufbuf = this->parsers[binaryoperation + 1];
                        if(bufbuf->pEnum == identifier)
                        {
                            auto _this = object;
                            object = object->getMember(*bufbuf->name);
                            index++;
                            binaryoperation++;

                            if(object!=nullptr && object is _BuiltFunc && this->parsers.size() > binaryoperation +1)
                            {
                                auto bufbuf = this->parsers[binaryoperation + 1];
                                if(bufbuf->pEnum == leftparent)
                                {
                                    index++;
                                    binaryoperation++;
                                    j = index;
                                    i = this->parentSkip(binaryoperation);
                                    std::vector<langObject> arg;
                                    index = index + 2;
                                    while (index<i)
                                    {
                                        arg.push_back(eval(NULLOBJECT,index,17));
                                        index++;
                                        if(this->parsers[index]->pEnum==parserEnum::comma)index++;
                                    }
                                    index = i;
                                    binaryoperation = index + 1;
                                    try
                                    {
                                        object = langObject(static_cast<SpecialFunction*>(object)->call(_this, &arg));
                                    }
                                    catch(langRuntimeException ex)
                                    {
                                        throw langRuntimeException(ex.what(),ex.tokens,ex.funcstacktrace,static_cast<Function*>(object)->name.c_str(),ex.stacktrace);
                                    }
                                }
                            }
                        }
                    }
                    //throw lang::langRuntimeException(".はClass型でのみ有効です。");
                }
                //i = index + 2;
                OP3
                    //if(this->parsers.size() > index + 1 && Operator(this->parsers[index + 1]->pEnum) >= thisop) object = eval(object, i, 17, true), index = i;
                    break;
            }
        }
        return langObject(object);
    }
}
