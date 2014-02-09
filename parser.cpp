#pragma once
#include "stdafx.h"
#include "parser.h"
#include <stack>
#include "Function.h"
#include "GC.h"
//#include "parserEnum.h"
#define newFunction(a,a1,a2,a3) new Function(a,a1,a2,a3)
namespace lang
{
enum parserStatus
{
	None,
	ReadIden,
	ReadNum,
	ReadStr,
};

bool isIden(char c)
{
    return (c>='A'&&c<='Z')||(c>='a'&&c<='z')||(c>='0'&&c<='9')||(c=='_');
}
bool isNum(char c)
{
    return c>='0'&&c<='9';
}

void parser::function()
{
    this->runner = new scope(this->parsers);
    lang::gc = new がべこれ(this->runner);
    int funcRead = 0;
    std::string* funcName = nullptr;
    std::vector<std::string>* argList = new std::vector<std::string>();
    std::stack<std::string> funcStack;
    int func = 0;
    for(int i=0;i<this->parsers.size();i++)
    {
        auto token = this->parsers[i];
        switch (token->pEnum)
        {
        case parserEnum::blockstart:
            if(funcRead != 6) funcStack.push("");
            break;
        case parserEnum::blockend:
            if(funcStack.size() !=0)
            {
                if(funcStack.top()!="")func--;
                funcStack.pop();
                break;
            }
        }
        switch (funcRead)
        {
        case 0://type
            if(token->pEnum == parserEnum::identifier)
            {
            if(*token->name == "return")
            {
            break;
            }
                funcRead++;
            }
            break;
        case 1://name
            if(token->pEnum == parserEnum::identifier)
            {
                funcRead++;
                funcName = token->name;
            }else funcRead = 0;
            break;
        case 2://(
            if(token->pEnum == parserEnum::leftparent)funcRead++;else funcRead = 0;
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
            else funcRead = 0;
            break;
        case 5://, or )
            if(token->pEnum == parserEnum::comma)funcRead-=2;
            else if(token->pEnum == parserEnum::rightparent)funcRead++; 
            else funcRead = 0;
            break;
        case 6://{
            funcStack.push(*funcName);
            std::cout<<std::endl;
            for(int j=0;j<funcStack.size();j++)
            {
                std::cout<<" ";
            }
            std::cout<<*funcName;
            funcRead = 0;
            if(func == 0)
            {
                this->runner->variable.add(*funcName,newFunction(funcName, argList, this->runner,i));
            }
            std::vector<std::string>* argList = new std::vector<std::string>();
            func++;
            //this->runner->variable.add(*funcName,std::make_shared<langFunction>(*funcName,argList,this->runner));
            break;
        }
    }
    std::cout<<std::endl;
}
parser::parser(std::string input)
{
	this->program = *(new std::string(input));
	std::cout << "Now Parsing...";
	auto sts = parserStatus::None;
    auto iden = new std::string();
	for(int i=0;i<=input.size();i++)
	{
        bool islast=i>=input.size()-1;
		auto chr = input[i];
		    char nextchr;
            if(!islast)nextchr = input[i+1];
        switch (sts)
        {
        case None:None:
            if(isNum(chr))
            {
                sts = parserStatus::ReadNum;
                goto ReadNum;
            }
            else
            if(isIden(chr))
            {
                sts = parserStatus::ReadIden;
                goto ReadIden;
            }
            switch (chr)
            {
            case '(':
                this->parsers.push_back(new parseObj(parserEnum::leftparent,new std::string("(")));
            break;
            case ')':
                this->parsers.push_back(new parseObj(parserEnum::rightparent,new std::string(")")));
            break;
            case '+':
                if(nextchr == '+')
                    this->parsers.push_back(new parseObj(parserEnum::plusplus,new std::string("++"))),i++;
                else
                    this->parsers.push_back(new parseObj(parserEnum::plus,new std::string("+")));
            break;
            case '-':
                this->parsers.push_back(new parseObj(parserEnum::minus,new std::string("-")));
            break;
            case '*':
                this->parsers.push_back(new parseObj(parserEnum::multiply,new std::string("*")));
            break;
            case '%':
                this->parsers.push_back(new parseObj(parserEnum::modulo,new std::string("%")));
            break;
            case ',':
                this->parsers.push_back(new parseObj(parserEnum::comma,new std::string(",")));
            break;
            case '=':if(nextchr == '=')
                    this->parsers.push_back(new parseObj(parserEnum::equalequal,new std::string("=="))),i++;
                else this->parsers.push_back(new parseObj(parserEnum::equal,new std::string("=")));
            break;
            case ';':
                this->parsers.push_back(new parseObj(parserEnum::semicolon,new std::string(";")));
            break;
            case '<':
                if(nextchr == '=')
                    this->parsers.push_back(new parseObj(parserEnum::lessequal,new std::string("<="))),i++;
                else this->parsers.push_back(new parseObj(parserEnum::less,new std::string("<")));
            break;
            case '>':
                if(nextchr == '=')
                    this->parsers.push_back(new parseObj(parserEnum::greaterequal,new std::string(">="))),i++;
                else 
                this->parsers.push_back(new parseObj(parserEnum::greater,new std::string(">")));
            break;
            case '{':
                this->parsers.push_back(new parseObj(parserEnum::blockstart,new std::string("{")));
            break;
            case '}':
                this->parsers.push_back(new parseObj(parserEnum::blockend,new std::string("}")));
            break;
            case '"':
                sts = parserStatus::ReadStr;
            break;
            case '\r':case '\n':case '\t':
            case ' ':break;
            default:
                this->parsers.push_back(new parseObj(parserEnum::none,new std::string(input.substr(i,1))));
                break;
            }
            break;
        case ReadIden:
        ReadIden:
            if(!isIden(chr))
            {
                this->parsers.push_back(new parseObj(parserEnum::identifier,iden));
                iden = new std::string();//!!!!コピーされないのでnew する!!!!
                sts = parserStatus::None;
                goto None;
            }
            //std::string chrs(chr);
            iden->append(input.substr(i,1));
            break;
        case ReadNum:ReadNum:
            if(!isNum(chr))
            {
                this->parsers.push_back(new parseObj(std::atoi(iden->c_str())));
                iden->clear();//!!!!コピーされるのでclear する!!!!
                sts = parserStatus::None;
                goto None;
            }
            //std::string chrs(chr);
            iden->append(input.substr(i,1));
            break;
        case ReadStr:ReadStr:
            if(chr=='"')
            {
                this->parsers.push_back(new parseObj(iden->c_str()));
                iden->clear();//!!!!コピーされるのでclear する!!!!
                sts = parserStatus::None;
            }
            else
            {
                if(chr=='\n'||chr=='\0')throw std::exception::exception("閉じられていないstring");
                iden->append(input.substr(i,1));
            }
            break;
        default:
            break;
        }
		std::cout << chr;
	}
    this->function();
	std::cout << "Done" << std::endl;
}


parser::~parser(void)
{
    try
    {
	//delete(&this->program);
	//delete &this->parsers;
    }
    catch(...)
    {
    }
}
}
