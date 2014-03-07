#pragma once
#include "stdafx.h"
#include "parser.h"
#include <stack>
#include "Function.h"
#include "GC.h"
#include "langException.h"
#include "Class.h"
#include "†.h"
//#include "parserEnum.h"
#define newFunction(a,a1,a2,a3) new Function(a,a1,a2,a3)
std::string てかＬＩＮＥやってる？(std::string input,int index);
namespace lang
{
#define HASHCLASS   1107
#define HASHTHIS    659
#define HASHNEW     339
#define HASHNAMESPACE 3721
#define HASHUSING   1067
#define HASHSTATIC  1573
#define HASHTRUE    651
#define HASHFALSE   1062
#define HASHNULL    657
#define ERROR(a) WARNING(a,0)//langObject NULLOBJECT = newObject(nullptr);
    enum class parserStatus
    {
        None,
        ReadIden,
        ReadNum,
        ReadStr,
        ReadComment,
        ReadBlockComment,
        ReadEscapeString,
        ReadChar,
        ReadEscapeChar,
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
        return (c>='A'&&c<='Z')||(c>='a'&&c<='z')||(c>='0'&&c<='9')||(c=='_')||c==':';//namespace
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
#define WARNINGS(level, ...) {char buf[512];sprintf_s(buf,__VA_ARGS__);WARNING(buf, level);}
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
    /**
    function
    class
    namespace
    を解析する。
    */
    enum class sts
    {
        Empty,      //   = 0,
        Func,       //   = 1,
        Class,      //   = 2,
        NameSpace,  //   = 4,
    };
    struct BlockStruct
    {
        sts type;
        std::string &name;
        BlockStruct(sts s,std::string &n):type(s),name(n){}
        BlockStruct(const BlockStruct& a) : name(a.name), type(a.type)
        {
        }
        BlockStruct& operator = (BlockStruct& a)
        {
            return BlockStruct(a);
        }
    };
    void parser::function()
    {
        this->runner = new scope(this->parsers);
        lang::gc = new がべこれ(this->runner);
        int funcRead = 0, classRead = 0;
        std::string funcName;std::string className;
        std::vector<std::string>* argList = nullptr;//new std::vector<std::string>();
        std::stack<BlockStruct> funcStack;
        int func = 0,parent = 0,bracket = 0;
        size_t class_read_stack_index = 0;
        std::string namesp;int namespread(0);
        membertype member = nullptr;
        membertype staticmember = nullptr;
        bool isstatic = false;
        for(int i=0;i<this->parsers.size();i++)
        {
            auto token = this->parsers[i];
            switch (namespread)
            {
            case 1:
                if(token->pEnum != parserEnum::identifier)
                    ERROR(("namespaceの名前が識別子ではありません"+てかＬＩＮＥやってる？(this->program,token->sourcestartindex)).c_str());
                //BUG!!
                //if(!namesp.empty())namesp += "::";//先頭にも::が付く
                namesp += *token->name;
                delete token->name;
                token->name = new std::string(namesp);
                namesp += "::";
                namespread++;
                break;
            case 2:
                namespread++;
                break;
            case 3:
                namespread = 0;
                break;
            }
            switch (token->pEnum)
            {
            case parserEnum::blockstart:
                if(funcRead != 6)
                {
                    if(namespread == 0)
                    {
                        funcStack.push(BlockStruct(sts::Empty,std::string()));
                    }
                    else
                        funcStack.push(BlockStruct(sts::NameSpace,namesp));
                }/*else 
                 func++;*/
                break;
            case parserEnum::blockend:
                if(funcStack.size() !=0)
                {
                    if(funcStack.top().type == sts::NameSpace)
                    {
                        namesp.clear();
                        auto cont = funcStack._Get_container();
                        if( funcStack.size()>=2)for(size_t i = funcStack.size() - 2;i>=0;i--)
                        {
                            if(cont[i].type == sts::NameSpace)
                            {
                                namesp = cont[i].name;
                                break;
                            }
                        }
                    }
                    if(funcStack.top().type == sts::Func) func--;
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
            case parserEnum::_namespace:
                namespread++;
                break;
            }
            switch (classRead)
            {
            case 0:
                if(token->pEnum == _class)
                {
                    if(funcRead != 0)
                        ERROR((てかＬＩＮＥやってる？(this->program,token->sourcestartindex) + "関数解析中のクラス").c_str());// WARNING("関数解析中のクラス");
                    if(funcStack.size() !=0 && funcStack.top().type == sts::Empty)
                        ERROR((てかＬＩＮＥやってる？(this->program,token->sourcestartindex) + "ネストされたクラス").c_str());
                    classRead++;
                }
                break;
            case 1:
                if(token->pEnum != identifier)
                    ERROR(("クラスの名前が識別子ではありません"+てかＬＩＮＥやってる？(this->program,token->sourcestartindex)).c_str());
                className = namesp + *token->name;
                if(member == nullptr)member = new membertype_();else delete member;
                if(staticmember == nullptr)staticmember = new membertype_();else delete staticmember;
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

                    if(token->pEnum == _static)
                    {
                        isstatic = true;
                    }
                }
                if(class_read_stack_index > funcStack.size())
                    if(token->pEnum == blockend)
                    {
                        this->runner->variable.add(className,newClass(className,0,member,this->runner,staticmember));
                        member = nullptr;
                        staticmember = nullptr;
                        classRead = 0;
                    }
                    break;
            case 4:
                if(token->pEnum == identifier)
                {
                    if(i + 1 < this->parsers.size() && this->parsers[i + 1]->pEnum == semicolon)
                    {
                        if(isstatic)
                            staticmember->push_back(std::pair<std::string,langObject>(*token->name, NULLOBJECT));
                        else
                            member->push_back(std::pair<std::string,langObject>(*token->name, NULLOBJECT));
                        isstatic = false;
                        classRead = 3;
                    }
                    if(i + 1 < this->parsers.size() && this->parsers[i + 1]->pEnum == equal)
                    {
                        if(isstatic)
                        {
                            staticmember->push_back(std::pair<std::string,langObject>(*token->name, NULLOBJECT));
                            staticmemberevals.push_back(std::tuple<int,std::string&,std::string>(i + 2,*token->name,className));
                        }else
                            std::cout<<"未実装"<<std::endl,member->push_back(std::pair<std::string,langObject>(*token->name, NULLOBJECT));
                        isstatic = false;
                        classRead = 3;
                    }
                    if(i + 1 < this->parsers.size() && this->parsers[i + 1]->pEnum == leftparent)
                    {
                        classRead = 3;
                        //isstatic = false;
                    }
                }
                else if(token->pEnum == leftparent)
                {
                    funcRead = 2;
                    funcName = "ctor";
                    classRead = 3;
                    isstatic = false;
                }
                classRead = 3;
                //isstatic = false;
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
                funcStack.push(BlockStruct(sts::Func,funcName));
#if _DEBUG
                if(lang::parserresult)
                {
                    std::cout<<std::endl;
                    for(int j=0;j<funcStack.size();j++)
                    {
                        std::cout<<" ";
                    }
                    std::cout<<namesp + funcName;
                }
#endif
                funcRead = 0;
                if(classRead == 3)
                {
                    if(isstatic)
                    {
                        staticmember->push_back(std::pair<std::string,langObject>(funcName, newFunction(funcName, argList, this->runner, i)));
                    }
                    else
                        member->push_back(std::pair<std::string,langObject>(funcName, newFunction(funcName, argList, this->runner, i)));
                }
                else if(func <= 0)
                {
                    this->runner->variable.add(namesp + funcName, newFunction(namesp + funcName, argList, this->runner, i));
                }
                else
                    delete argList;//とりあえず
                argList = nullptr;//new std::vector<std::string>();
                func++;
                //this->runner->variable.add(*funcName,std::make_shared<langFunction>(*funcName,argList,this->runner));
                isstatic = false;
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
    void parser::namespaceparse()
    {
        std::string namesp;
        std::stack<BlockStruct> funcStack;
        std::string empty;
        for(int i=0;i<this->parsers.size();i++)
        {
            auto token = this->parsers[i];
            parseObj* nextoken = nullptr;
            if(i + 1 < this->parsers.size()) nextoken = this->parsers[i + 1];
            switch (token->pEnum)
            {
            case parserEnum::_using:
                if(nextoken == nullptr) 
                {
                    ERROR("不正なusing");
                    break;
                }
                this->usings.push_back(*nextoken->name);
                i++;
                break;
                //odentifier
            case parserEnum::identifier:
                if(!DEFINEDSCPEVAR(this->runner, *token->name))
                {
                    //捜す
                    bool success = false;
                    auto name = namesp + *token->name;
                    if(DEFINEDSCPEVAR(this->runner, name))
                    {
                        delete token->name;
                        token->name = new std::string(name);
                        break;
                    }
                    else
                    {
                        for(auto i : this->usings)
                        {
                            //捜す
                            auto name = i + "::" + *token->name;
                            if(DEFINEDSCPEVAR(this->runner, name))
                            {
                                delete token->name;
                                token->name = new std::string(name);
                                success = true;
                                break;
                            }
                        }
                    }
                    //if(!success)WARNINGS(1, "%sが見つからない", token->name->c_str());
                }
                break;
            case parserEnum::_namespace:
                namesp = *nextoken->name + "::";
                break;
            case parserEnum::blockstart:
                if(1 < i) 
                {
                    auto prevtoken = this->parsers[i - 2];
                    if(prevtoken->pEnum == parserEnum::_namespace)
                    {
                        funcStack.push(BlockStruct(sts::NameSpace, namesp));
                    }
                    else funcStack.push(BlockStruct(sts::Empty, empty));
                }else funcStack.push(BlockStruct(sts::Empty, empty));
                break;
            case parserEnum::blockend:
                if(funcStack.size() !=0)
                {
                    if(funcStack.top().type == sts::NameSpace)
                    {
                        namesp.clear();
                        auto cont = funcStack._Get_container();
                        if(funcStack.size() >= 2) for(size_t i = funcStack.size() - 2;i>=0;i--)
                        {
                            if(cont[i].type == sts::NameSpace)
                            {
                                namesp = cont[i].name;
                                break;
                            }
                        }
                    }
                    funcStack.pop();
                    break;
                }
                break;
            }
        }
        for(int i=0;i<this->staticevals.size();i++)
        {
            int j = this->staticevals[i];
            this->runner->eval(this->parsers[j]->ptr,j);
        }
        foreach_(auto &&i in_ this->staticmemberevals)
        {
            ((langClass)this->runner->variable[std::get<2>(i)])->thisscope->variable.set(std::get<1>(i),this->runner->eval(NULLOBJECT,std::get<0>(i)));
        }
    }
    void parser::staticparse()
    {
        std::stack<BlockStruct> funcStack;
        std::string namesp;
        std::string varname;
        int read = 0,block = 0;
        bool isclass = false;
        std::string empty;
        for(int i=0;i<this->parsers.size();i++)
        {
            auto token = this->parsers[i];
            parseObj* nextoken = nullptr;
            if(i + 1 < this->parsers.size()) nextoken = this->parsers[i + 1];
            switch(read)
            {
            case 0: //static
                if(token->pEnum == parserEnum::_static)
                {
                    read++;
                }
                break;
            case 1: //name
                varname = namesp + *token->name;
                delete token->name;
                token->name = new std::string(varname);
                this->runner->variable.add(varname, NULLOBJECT);
                read++;
                break;
            case 2: //=
                if(token->pEnum == parserEnum::equal)
                {
                    read++;
                }
                else read = 0;
                break;
            case 3:  //exp1517754
                staticevals.push_back(i - 2);
                read =0;
                break;
            case -1:
                read = -2;
                break;
            case -2:
                if(token->pEnum == parserEnum::blockstart)block++;
                if(token->pEnum == parserEnum::blockend)block--;
                if(block == 0) read = 0;
                break;
            }
            switch (token->pEnum)
            {
            case parserEnum::_class:
                read = -1;
                break;
            case parserEnum::_namespace:
                namesp = *nextoken->name + "::";
                break;
            case parserEnum::blockstart:
                if(1 < i) 
                {
                    auto prevtoken = this->parsers[i - 2];
                    if(prevtoken->pEnum == parserEnum::_namespace)
                    {
                        funcStack.push(BlockStruct(sts::NameSpace, namesp));
                    }
                    else 

                        if(prevtoken->pEnum == parserEnum::_class)
                        {
                            funcStack.push(BlockStruct(sts::Class, *this->parsers[i - 1]->name));
                        }
                        else funcStack.push(BlockStruct(sts::Empty, empty));
                }else funcStack.push(BlockStruct(sts::Empty, empty));
                break;
            case parserEnum::blockend:
                if(funcStack.size() !=0)
                {
                    if(funcStack.top().type == sts::NameSpace)
                    {
                        namesp.clear();
                        auto cont = funcStack._Get_container();
                        if( funcStack.size() >= 2)for(size_t i = funcStack.size() - 2;i>=0;i--)
                        {
                            if(cont[i].type == sts::NameSpace)
                            {
                                namesp = cont[i].name;
                                break;
                            }
                        }
                    }
                    funcStack.pop();
                    break;
                }
                break;
            }
        }
    }
    parser::parser(std::string input)
    {
        int line = 0;
        lang::gc = nullptr;
        this->program = (input);
#if _DEBUG
        if(lang::parserresult) std::cout << "Now Parsing...";
#endif
        auto sts = parserStatus::None;
        auto iden = new std::string();
        bool shiftJis = false, ASCII = true;
        int startindex = 0;
        int blockComment = 0;
        bool isWChar = false;
        for(int i=0;i<=input.size();i++)
        {
            bool islast = i >= input.size() - 1;
            bool isfast = i == 0;
            auto chr = input[i];
            char nextchr;
            char prevchr;
            if(!islast)nextchr = input[i + 1];
            if(!isfast)prevchr = input[i - 1];
            switch (sts)
            {
            case parserStatus::None:None:
                isWChar = false;
                switch (chr)
                {
                case '(':
                    this->parsers.push_back(new parseObj(parserEnum::leftparent,new std::string("("), i, i));
                    break;
                case ')':
                    this->parsers.push_back(new parseObj(parserEnum::rightparent,new std::string(")"), i, i));
                    break;
                case '[':
                    this->parsers.push_back(new parseObj(parserEnum::leftbracket,new std::string("["), i, i));
                    break;
                case ']':
                    this->parsers.push_back(new parseObj(parserEnum::rightbracket,new std::string("]"), i, i));
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
                    else if(nextchr == '<')
                        this->parsers.push_back(new parseObj(parserEnum::leftshift,new std::string("<<"), i, i + 1)),i++;
                    else this->parsers.push_back(new parseObj(parserEnum::less,new std::string("<"), i, i));
                    break;
                case '>':
                    if(nextchr == '=')
                        this->parsers.push_back(new parseObj(parserEnum::greaterequal,new std::string(">="), i, i + 1)),i++;
                    else if(nextchr == '>')
                        this->parsers.push_back(new parseObj(parserEnum::rightshift,new std::string(">>"), i, i + 1)),i++;
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
                case 'L':
                    if(nextchr != '\'') break;
                    isWChar = true;
                    i++;
                case '\'':
                    startindex = i;
                    sts = parserStatus::ReadChar;
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
                case '\n': line++;
                case '\r':case '\t':
                case ' ':break;
                default:
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
                            case HASHNAMESPACE:
                                if(*iden == "namespace"){this->parsers.push_back(new parseObj(parserEnum::_namespace,iden, startindex, i - 1));ok = true;}
                                break;
                            case HASHUSING:
                                if(*iden == "using"){this->parsers.push_back(new parseObj(parserEnum::_using,iden, startindex, i - 1));ok = true;}
                                break;
                            case HASHSTATIC:
                                if(*iden == "static"){this->parsers.push_back(new parseObj(parserEnum::_static,iden, startindex, i - 1));ok = true;}
                                break;
                            case HASHTRUE:
                                if(*iden == "true"){this->parsers.push_back(new parseObj(parserEnum::_true,iden, startindex, i - 1, TRUEOBJECT));ok = true;}
                                break;
                            case HASHFALSE:
                                if(*iden == "false"){this->parsers.push_back(new parseObj(parserEnum::_false,iden, startindex, i - 1, FALSEOBJECT));ok = true;}
                                break;
                            case HASHNULL:
                                if(*iden == "null"){this->parsers.push_back(new parseObj(parserEnum::_null,iden, startindex, i - 1,NULLOBJECT));ok = true;}
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
                    if(chr == '\\' && !(shiftJis && isIdenShiftJIS1(prevchr)))
                    {
                        sts = parserStatus::ReadEscapeString;
                    }else
                        if(chr=='\n'||chr=='\0')
                        {
                            WARNING("閉じられていないstring");//throw langParseException("閉じられていないstring");
                        }else
                            iden->append(input.substr(i,1));
                }
                break;
            case parserStatus::ReadChar:ReadChar:
                if(chr=='\'')
                {
                    if(iden->size() < 1 || iden->size() > 1)
                    {
                        if(iden->size() == 2)
                        {
                            if(!isWChar) 
                            {
                                WARNINGS(0,"charのsizeは1ですWCharに変更します。L\'%s\'", iden->c_str());
                                isWChar = true;
                            }
                        }
                        else
                            WARNINGS(0,"charのsizeは1です%s", iden->c_str());
                    }
                    if(isWChar)
                    {
                        int size = iden->size();
                        const char* wStrC = iden->c_str();
                        //変換文字列格納バッファ
                        wchar wStrW[2];
                        size_t wLen = 0;
                        errno_t err = 0;
                        //ロケール指定
                        setlocale(LC_ALL,"japanese");
                        //変換
                        err = mbstowcs_s(&wLen, /*&*/wStrW, size, wStrC, _TRUNCATE);
                        this->parsers.push_back(new parseObj(wStrW[0], startindex, i));
                    }
                    else
                    {
                        this->parsers.push_back(new parseObj((*iden)[0], startindex, i));

                    }
                    iden->clear();

                    sts = parserStatus::None;
                }
                else
                {
                    if(chr == '\\' && !(shiftJis && isIdenShiftJIS1(prevchr)))
                    {
                        sts = parserStatus::ReadEscapeChar;
                    }else
                        if(chr=='\n'||chr=='\0')
                        {
                            WARNING("閉じられていないchar");//throw langParseException("閉じられていないstring");
                        }else
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
            case parserStatus::ReadEscapeString:
                switch (chr)
                {
                case 'n':
                    *iden += '\n';
                    break;
                case 't':
                    *iden +='\t';
                    break;
                default:
                    WARNING((std::string("認識できないエスケープシーケンス") + chr).c_str(),1);
                    break;
                }
                sts = parserStatus::ReadStr;
                break;
            case parserStatus::ReadEscapeChar:
                switch (chr)
                {
                case 'n':
                    *iden += '\n';
                    break;
                case 't':
                    *iden +='\t';
                    break;
                default:
                    WARNING((std::string("認識できないエスケープシーケンス") + chr).c_str(),1);
                    break;
                }
                sts = parserStatus::ReadChar;
                break;
            default:
                throw langParseException("WHAT???");
                break;
            }
#if _DEBUG
            if(lang::parserresult) std::cout << chr;
#endif
        }
        if(iden->empty())delete iden;//使われてないからdelete
        lang::plugin::†(this->parsers);
        this->function();
        this->staticparse();
        this->namespaceparse();
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
