#ifndef __LANG_EXCEPTION_H__
#include <stdexcept>
#define __LANG_EXCEPTION_H__
//#pragma once
namespace lang
{
	class parseObj;
    class langException : public std::runtime_error
    {
    public:
        explicit langException(const char* _Message)
            : runtime_error(_Message)
        {	// construct from message string
        }
    };
	class langExitException : public langException
	{
	public:
		int code;
		langExitException(int code) : langException("lang::langExitException")
		{
			this->code = code;
		}
	};
    class langRuntimeException : public langException
    {
    public:
        int index;
        int eindex;
        std::vector<parseObj*> tokens;
        std::vector<std::pair<int,int>> stacktrace;
        std::vector<const char*> funcstacktrace;
        explicit langRuntimeException(const char* _Message,int index,int eindex,std::vector<parseObj*>& v,std::vector<std::pair<int,int> >& s,std::vector<const char*>& fs)
            : langException(_Message)
        {	// construct from message string
            this->index = index;
            this->eindex = eindex;
            this->tokens = v;
            this->stacktrace = s;
            this->stacktrace.push_back(std::pair<int,int>(index,eindex));
            this->funcstacktrace = fs;
        }
        explicit langRuntimeException(const char* _Message,std::vector<parseObj*>& v,std::vector<const char*>& fs,const char* funcname,std::vector<std::pair<int,int> >& s)
            : langException(_Message)
        {	// construct from message string
            this->tokens = v;
            this->funcstacktrace = fs;
            this->funcstacktrace.push_back(funcname);
            this->stacktrace = s;
        }
        explicit langRuntimeException(const char* _Message)
            : langException(_Message)
        {	// construct from message string
        }
	};
	class langUserException : public langException
	{
	public:
		langObject object;
		langUserException(langObject object) : langException("langUserException")
		{
			this->object = object;
		}
	};
#ifdef _WIN32
#define throw_langRuntimeException(...) {char buf[512];sprintf_s(buf,__VA_ARGS__);throw langRuntimeException(buf);}
#else
#define throw_langRuntimeException(...) {char buf[512];sprintf(buf,__VA_ARGS__);throw langRuntimeException(buf);}
#endif
    class langParseException : public langException
    {
    public:
        explicit langParseException(const char* _Message)
            : langException(_Message)
        {	// construct from message string
        }
    };
}
#endif
