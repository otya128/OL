#include "stdafx.h"
#include "dagger.h"

namespace lang
{
    namespace plugin
    {
        //daggerö
		dagger::dagger(std::vector<parseObj*>& p) : tokens(p)
        {
        #ifdef CPP11
            auto old = tokens;//copy
			parseObj* shiftjisdagger = nullptr;
            tokens.clear();
            foreach_(var_ i in_ old)
            {
				if (shiftjisdagger)
                {
                    if(i->pEnum == parserEnum::none && i->name->size() == 1 && (*i->name)[0] == (char)-11)
                    {
                        tokens.push_back(new parseObj(parserEnum::leftparent,new std::string("dagger"),i->sourcestartindex,i->sourceendindex));
                        delete i;
                            delete shiftjisdagger;
                    }
                    else
                    {
                        if(i->pEnum == parserEnum::none && i->name->size() == 1 && (*i->name)[0] == (char)-10)
                        {
                            tokens.push_back(new parseObj(parserEnum::rightparent,new std::string("ö"),i->sourcestartindex,i->sourceendindex));
                            delete i;
                            delete shiftjisdagger;
                        }
                        else
                        {
                            tokens.push_back(shiftjisdagger);
                            tokens.push_back(i);
                        }
                    }
                    shiftjisdagger = nullptr;
                }
                else
                    if(i->pEnum == parserEnum::none && i->name->size() == 1 && (*i->name)[0] == (char)-127)
                    {
                        shiftjisdagger = i;
                        continue;
                    }
                    else
                    {
                        tokens.push_back(i);
                    }
            }
        #endif
        }


        dagger::~dagger(void)
        {
        }
    }
}
