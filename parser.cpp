#pragma once
#include "stdafx.h"
#include "parser.h"
#include <stack>
#include "Function.h"
#include "GC.h"
#include "langException.h"
#include "Class.h"
//#include "parserEnum.h"
#define newFunction(a,a1,a2,a3) new Function(a,a1,a2,a3)
std::string てかＬＩＮＥやってる？(std::string input,int index);
namespace lang
{
#define HASHCLASS   1107
#define HASHTHIS    659
#define HASHNEW     339
#define ERROR(a) WARNING(a,0)//langObject NULLOBJECT = newObject(nullptr);
    enum class parserStatus
    {
        None,
        ReadIden,
        ReadNum,
        ReadStr,
        ReadComment,
        ReadBlockComment,
    };
    int error_level;
    //文字列から単純なhashを作成
    //文字コード*位置を足していく単純なもの
    int aho_hash(const char* c)
    {
        int r = 0, index = 0;
        while(*(c++))
        {
            index++;
            r += (*c) * index;
        }
        return r;
    }
    bool isIden(unsigned char c)
    {
        return (c>='A'&&c<='Z')||(c>='a'&&c<='z')||(c>='0'&&c<='9')||(c=='_');
    }
    bool isIdenShiftJIS(unsigned char c)
    {
        return c>=0x80 && c<=0xFF;
    }
    //1byte目が全角文字かどうか
    bool isIdenShiftJIS1(unsigned char c)
    {
        return (c>=0x80 && c<=0x9F) || (c>=0xE0 && c<=0xFF);
    }
    bool isNum(char c)
    {
        return c>='0'&&c<='9';
    }
    void WARNING(const char* param, int level = 0)
    {
        if(level<=error_level)
        {
            std::cout<<"[ERROR]"<<param<<std::endl;
        }
        else
        {
            std::cout<<"[WARNING]"<<param<<std::endl;
        }
    }
    void parser::function()
    {
        this->runner = new scope(this->parsers);
        lang::gc = new がべこれ(this->runner);
        int funcRead = 0, classRead = 0;
        std::string funcName;std::string className;
        std::vector<std::string>* argList = nullptr;//new std::vector<std::string>();
        std::stack<std::string> funcStack;
        int func = 0,parent = 0,bracket = 0;
        int class_read_stack_index = 0;
        membertype member = nullptr;
        for(int i=0;i<this->parsers.size();i++)
        {
            auto token = this->parsers[i];
            switch (token->pEnum)
            {
            case parserEnum::blockstart:
                if(funcRead != 6)
                {
                    func++;
                    funcStack.push("");
                }
                break;
            case parserEnum::blockend:
                if(funcStack.size() !=0)
                {
                    if(funcStack.top()!="")func--;
                    funcStack.pop();
                    break;
                }
                break;
            case parserEnum::leftparent:
                parent++;
                break;
            case parserEnum::rightparent:
                parent--;
                break;
            case parserEnum::leftbracket:
                bracket++;
                break;
            case parserEnum::rightbracket:
                bracket--;
                break;
            }
            switch (classRead)
            {
            case 0:
                if(token->pEnum == _class)
                {
                    if(funcRead != 0)
                        ERROR((てかＬＩＮＥやってる？(this->program,token->sourcestartindex) + "関数解析中のクラス").c_str());// WARNING("関数解析中のクラス");
                    if(funcStack.size() !=0 && funcStack.top()=="")
                        ERROR((てかＬＩＮＥやってる？(this->program,token->sourcestartindex) + "ネストされたクラス").c_str());
                    classRead++;
                }
                break;
            case 1:
                if(token->pEnum != identifier)
                    ERROR(("クラスの名前が識別子ではありません"+てかＬＩＮＥやってる？(this->program,token->sourcestartindex)).c_str());
                className = *token->name;
                if(member == nullptr)member = new membertype_();else delete member;
                classRead++;
                break;
            case 2:
                if(token->pEnum != blockstart)
                    ERROR(("class " + className + "{で宣言する必要があります。"+てかＬＩＮＥやってる？(this->program,token->sourcestartindex)).c_str());
                classRead++;
                class_read_stack_index = funcStack.size();
                break;
            case 3:
                if(class_read_stack_index == funcStack.size())
                {
                    if(token->pEnum == identifier)
                    {
                        if(*token->name == "return")
                        {
                            break;
                        }
                        classRead++;
                    }
                }
                if(class_read_stack_index > funcStack.size())
                    if(token->pEnum == blockend)
                    {
                        this->runner->variable.add(className,newClass(className,0,member,this->runner));
                        member = nullptr;
                    }
                    break;
            case 4:
                if(token->pEnum == identifier)
                {
                    if(i+1<this->parsers.size()&& this->parsers[i+1]->pEnum == semicolon)
                    {
                        member->push_back(std::pair<std::string,langObject>(*token->name, NULLOBJECT));
                        classRead = 3;
                    }
                    if(i+1<this->parsers.size()&& this->parsers[i+1]->pEnum == leftparent)
                    {
                        classRead = 3;
                    }
                }
                break;
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
                    funcName = *token->name;
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
                    if(argList == nullptr)argList = new std::vector<std::string>();
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
                if(argList == nullptr)argList = new std::vector<std::string>();
                funcStack.push(funcName);
                std::cout<<std::endl;
                for(int j=0;j<funcStack.size();j++)
                {
                    std::cout<<" ";
                }
                std::cout<<funcName;
                funcRead = 0;
                if(classRead == 3)
                {
                    member->push_back(std::pair<std::string,langObject>(funcName, newFunction(funcName, argList, this->runner,i)));
                }
                else if(func == 0)
                {
                    this->runner->variable.add(funcName,newFunction(funcName, argList, this->runner,i));
                }
                argList = nullptr;//new std::vector<std::string>();
                func++;
                //this->runner->variable.add(*funcName,std::make_shared<langFunction>(*funcName,argList,this->runner));
                break;
            }
        }
        if(parent != 0)
        {
            WARNING("一致しない括弧(");
        }
        if(bracket != 0)
        {
            WARNING("一致しない括弧[");
        }
        std::cout<<std::endl;
    }
    parser::parser(std::string input)
    {
        lang::gc = nullptr;
        this->program = (input);
#if _DEBUG
        if(lang::parserresult) std::cout << "Now Parsing...";
#endif
        auto sts = parserStatus::None;
        auto iden = new std::string();
        bool shiftJis = true, ASCII = false;
        int startindex = 0;
        int blockComment = 0;
        for(int i=0;i<=input.size();i++)
        {
            bool islast=i>=input.size()-1;
            bool isfast = i==0;
            auto chr = input[i];
            char nextchr;
            char prevchr;
            if(!islast)nextchr = input[i+1];
            if(!isfast)prevchr = input[i-1];
            switch (sts)
            {
            case parserStatus::None:None:
                if(isNum(chr))
                {
                    startindex = i;
                    sts = parserStatus::ReadNum;
                    goto ReadNum;
                }
                else
                    if(isIden(chr))
                    {
                        startindex = i;
                        sts = parserStatus::ReadIden;
                        goto ReadIden;
                    }
                    else if(shiftJis && isIdenShiftJIS(chr))
                    {
                        startindex = i;
                        sts = parserStatus::ReadIden;
                        goto ReadIden;
                    }
                    switch (chr)
                    {
                    case '(':
                        this->parsers.push_back(new parseObj(parserEnum::leftparent,new std::string("("), i, i));
                        break;
                    case ')':
                        this->parsers.push_back(new parseObj(parserEnum::rightparent,new std::string(")"), i, i));
                        break;
                    case '+':
                        if(nextchr == '+')
                            this->parsers.push_back(new parseObj(parserEnum::plusplus,new std::string("++"), i, i + 1)),i++;
                        else
                            this->parsers.push_back(new parseObj(parserEnum::plus,new std::string("+"), i, i));
                        break;
                    case '-':
                        this->parsers.push_back(new parseObj(parserEnum::minus,new std::string("-"), i, i));
                        break;
                    case '*':
                        this->parsers.push_back(new parseObj(parserEnum::multiply,new std::string("*"), i, i));
                        break;
                    case '%':
                        this->parsers.push_back(new parseObj(parserEnum::modulo,new std::string("%"), i, i));
                        break;
                    case ',':
                        this->parsers.push_back(new parseObj(parserEnum::comma,new std::string(","), i, i));
                        break;
                    case '.':
                        this->parsers.push_back(new parseObj(parserEnum::dot,new std::string("."), i, i));
                        break;
                    case '=':if(nextchr == '=')
                                 this->parsers.push_back(new parseObj(parserEnum::equalequal,new std::string("=="), i, i + 1)),i++;
                             else this->parsers.push_back(new parseObj(parserEnum::equal,new std::string("="), i, i));
                             break;
                    case ';':
                        this->parsers.push_back(new parseObj(parserEnum::semicolon,new std::string(";"), i, i));
                        break;
                    case '<':
                        if(nextchr == '=')
                            this->parsers.push_back(new parseObj(parserEnum::lessequal,new std::string("<="), i, i + 1)),i++;
                        else this->parsers.push_back(new parseObj(parserEnum::less,new std::string("<"), i, i));
                        break;
                    case '>':
                        if(nextchr == '=')
                            this->parsers.push_back(new parseObj(parserEnum::greaterequal,new std::string(">="), i, i + 1)),i++;
                        else 
                            this->parsers.push_back(new parseObj(parserEnum::greater,new std::string(">"), i, i));
                        break;
                    case '{':
                        this->parsers.push_back(new parseObj(parserEnum::blockstart,new std::string("{"), i, i));
                        break;
                    case '}':
                        this->parsers.push_back(new parseObj(parserEnum::blockend,new std::string("}"), i, i));
                        break;
                    case '"':
                        startindex = i;
                        sts = parserStatus::ReadStr;
                        break;
                    case '/':
                        if(nextchr == '/')
                        {
                            sts = parserStatus::ReadComment;i++;
                        }
                        else 
                            if(nextchr == '*')
                            {
                                sts = parserStatus::ReadBlockComment;i++;blockComment++;
                            }
                            else
                                if(nextchr == '=')
                                    this->parsers.push_back(new parseObj(parserEnum::divisionequal,new std::string("/="), i, i + 1)),i++;
                                else this->parsers.push_back(new parseObj(parserEnum::division,new std::string("/"), i, i));
                                break;
                    case '\r':case '\n':case '\t':
                    case ' ':break;
                    default:
                        this->parsers.push_back(new parseObj(parserEnum::none,new std::string(input.substr(i,1)), i, i));
                        break;
                    }
                    break;
            case parserStatus::ReadIden:
ReadIden:
                if(!isIden(chr))
                {
                    if(!(shiftJis && isIdenShiftJIS(chr)))
                        if(!shiftJis || !isIdenShiftJIS1(prevchr))
                        {
                            int aho = aho_hash(iden->c_str());
                            bool ok = false;
                            //三回も文字列を比較するのは無駄そうだから適当にハッシュ値を計算して最大で二回分にしてみるテスト
                            switch (aho)
                            {
                            case HASHCLASS:
                                if(*iden == "class"){this->parsers.push_back(new parseObj(parserEnum::_class,iden, startindex, i - 1));ok = true;}
                                break;
                            case HASHNEW:
                                if(*iden == "new"){this->parsers.push_back(new parseObj(parserEnum::_new,iden, startindex, i - 1));ok = true;}
                                break;
                            case HASHTHIS:
                                if(*iden == "this"){this->parsers.push_back(new parseObj(parserEnum::_this,iden, startindex, i - 1));ok = true;}
                                break;
                            }
                            if(!ok) this->parsers.push_back(new parseObj(parserEnum::identifier,iden, startindex, i - 1));
                            iden = new std::string();//!!!!コピーされないのでnew する!!!!
                            sts = parserStatus::None;
                            goto None;
                        }
                }
                //std::string chrs(chr);
                iden->append(input.substr(i,1));
                break;
            case parserStatus::ReadNum:ReadNum:
                if(!isNum(chr))
                {
                    this->parsers.push_back(new parseObj(std::atoi(iden->c_str()), startindex, i - 1));
                    iden->clear();//!!!!コピーされるのでclear する!!!!
                    sts = parserStatus::None;
                    goto None;
                }
                //std::string chrs(chr);
                iden->append(input.substr(i,1));
                break;
            case parserStatus::ReadStr:ReadStr:
                if(chr=='"')
                {
                    this->parsers.push_back(new parseObj(*iden, startindex, i));
                    iden->clear();//!!!!コピーされるのでclear する!!!!
                    sts = parserStatus::None;
                }
                else
                {
                    if(chr=='\n'||chr=='\0')
                    {
                        WARNING("閉じられていないstring");//throw langParseException("閉じられていないstring");
                    }
                    iden->append(input.substr(i,1));
                }
                break;
            case parserStatus::ReadComment:
                if(chr=='\n'||chr=='\0') sts = parserStatus::None;
                break;
            case parserStatus::ReadBlockComment:
                if(chr == '*' && nextchr == '/') {blockComment--;i++;}
                if(chr == '/' && nextchr == '*') {blockComment++;i++;}
                if(!blockComment) sts = parserStatus::None;
                break;
            default:
                break;
            }
#if _DEBUG
            if(lang::parserresult) std::cout << chr;
#endif
        }
        if(iden->empty())delete iden;//使われてないからdelete
        this->function();
#if _DEBUG
        if(lang::parserresult)std::cout << "Done" << std::endl;
#endif
    }


    parser::~parser(void)
    {
        try
        {
            delete lang::gc;

            //delete this->runner;
            //delete(&this->program);
            //delete &this->parsers;
        }
        catch(...)
        {
        }
    }
}
