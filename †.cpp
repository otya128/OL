#include "stdafx.h"
#include "��.h"

namespace lang
{
    namespace plugin
    {
        //����
        ��::��(std::vector<parseObj*>& p) : tokens(p)
        {
        #ifdef CPP11
            auto old = tokens;//copy
            parseObj* shiftjis�� = nullptr;
            tokens.clear();
            foreach_(var_ i in_ old)
            {
                if(shiftjis��)
                {
                    if(i->pEnum == parserEnum::none && i->name->size() == 1 && (*i->name)[0] == (char)-11)
                    {
                        tokens.push_back(new parseObj(parserEnum::leftparent,new std::string("��"),i->sourcestartindex,i->sourceendindex));
                        delete i;
                            delete shiftjis��;
                    }
                    else
                    {
                        if(i->pEnum == parserEnum::none && i->name->size() == 1 && (*i->name)[0] == (char)-10)
                        {
                            tokens.push_back(new parseObj(parserEnum::rightparent,new std::string("��"),i->sourcestartindex,i->sourceendindex));
                            delete i;
                            delete shiftjis��;
                        }
                        else
                        {
                            tokens.push_back(shiftjis��);
                            tokens.push_back(i);
                        }
                    }
                    shiftjis�� = nullptr;
                }
                else
                    if(i->pEnum == parserEnum::none && i->name->size() == 1 && (*i->name)[0] == (char)-127)
                    {
                        shiftjis�� = i;
                        continue;
                    }
                    else
                    {
                        tokens.push_back(i);
                    }
            }
        #endif
        }


        ��::~��(void)
        {
        }
    }
}
