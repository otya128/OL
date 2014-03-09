#include "stdafx.h"
#include "õ.h"

namespace lang
{
    namespace plugin
    {
        //õö
        õ::õ(std::vector<parseObj*>& p) : tokens(p)
        {
        #ifdef CPP11
            auto old = tokens;//copy
            parseObj* shiftjisõ = nullptr;
            tokens.clear();
            foreach_(var_ i in_ old)
            {
                if(shiftjisõ)
                {
                    if(i->pEnum == parserEnum::none && i->name->size() == 1 && (*i->name)[0] == (char)-11)
                    {
                        tokens.push_back(new parseObj(parserEnum::leftparent,new std::string("õ"),i->sourcestartindex,i->sourceendindex));
                        delete i;
                            delete shiftjisõ;
                    }
                    else
                    {
                        if(i->pEnum == parserEnum::none && i->name->size() == 1 && (*i->name)[0] == (char)-10)
                        {
                            tokens.push_back(new parseObj(parserEnum::rightparent,new std::string("ö"),i->sourcestartindex,i->sourceendindex));
                            delete i;
                            delete shiftjisõ;
                        }
                        else
                        {
                            tokens.push_back(shiftjisõ);
                            tokens.push_back(i);
                        }
                    }
                    shiftjisõ = nullptr;
                }
                else
                    if(i->pEnum == parserEnum::none && i->name->size() == 1 && (*i->name)[0] == (char)-127)
                    {
                        shiftjisõ = i;
                        continue;
                    }
                    else
                    {
                        tokens.push_back(i);
                    }
            }
        #endif
        }


        õ::~õ(void)
        {
        }
    }
}
