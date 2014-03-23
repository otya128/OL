#pragma once
#include "stdafx.h"
#include <sstream>
#include "parser.h"
#include <stack>
#include "Function.h"
#include "GC.h"
#include "langException.h"
#include "Class.h"
#include "dagger.h"
//#include "parserEnum.h"
#define newFunction(a,a1,a2,a3) new Function(a,a1,a2,a3)
std::string getlinestring(std::string input, int index);
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
#define HASHBREAK   1035
#define HASHCONTINUE 3058
#define HASHELSE    641
#define HASHWHILE   1042
#define HASHFOR     339
#define HASHIF      102
#define HASHRETURN  1690
#define HASHBASE    630
#define HASHFOREACH 2149
#define HASHIN      110
#define HASHIS      115
#define HASHAS      115
#define ERROR(a) WARNING(a,0)//langObject NULLOBJECT = newObject(nullptr);
	enum ENUMCLASS parserStatus
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
		ReadDouble
	};
	int error_level;
	//�����񂩂�P����hash���쐬
	//�����R�[�h*�ʒu�𑫂��Ă����P���Ȃ���
	int aho_hash(const char* c)
	{
		int r = 0, index = 0;
		while (*(c++))
		{
			index++;
			r += (*c) * index;
		}
		return r;
	}
	bool isIden(unsigned char c, unsigned char nc)
	{
		return (c >= 'A'&&c <= 'Z') || (c >= 'a'&&c <= 'z') || (c >= '0'&&c <= '9') || (c == '_') || (c == ':');//namespace
	}
	bool isIdenShiftJIS(unsigned char c)
	{
		return c >= 0x80 && c <= 0xFF;
	}
	//1byte�ڂ��S�p�������ǂ���
	bool isIdenShiftJIS1(unsigned char c)
	{
		return (c >= 0x80 && c <= 0x9F) || (c >= 0xE0 && c <= 0xFF);
	}
	bool isNum(char c)
	{
		return c >= '0'&&c <= '9';
	}
#define WARNINGS(level, ...) {char buf[512];sprintf_s(buf,__VA_ARGS__);WARNING(buf, level);}
	void parser::WARNING(const char* param, int level)
	{
		if (level <= error_level)
		{
			errcount++;
			std::cerr << "[ERROR]" << param << std::endl;
		}
		else
		{
			std::cout << "[WARNING]" << param << std::endl;
		}
	}
	/**
	function
	class
	namespace
	����͂���B
	*/
	enum ENUMCLASS sts
	{
		Empty,      //   = 0,
		Func,       //   = 1,
		stsClass,   //   = 2,
		NameSpace,  //   = 4,
	};
	struct BlockStruct
	{
		sts type;
		std::string name;
		BlockStruct(sts s, std::string n) :type(s), name(n){}
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
		this->runner->variable.add("string", new StringType());
		this->runner->variable.add("object", new lang::ObjectType());
		this->runner->variable.add("int", new IntType());
		this->runner->variable.add("double", new DoubleType());
		this->runner->variable.add("char", new CharType());
		this->runner->variable.add("wchar", new WCharType());
		lang::gc = new ���ׂ���(this->runner);
		int funcRead = 0, classRead = 0;
		std::string funcName; std::string className;
		std::vector<std::string>* argList = nullptr;//new std::vector<std::string>();
		std::stack<BlockStruct> funcStack;
		int func = 0, parent = 0, bracket = 0;
		size_t class_read_stack_index = 0;
		std::string namesp; int namespread(0);
		int extendname;
		membertype member = nullptr;
		membertype staticmember = nullptr;
		bool isstatic = false;
		for (int i = 0; i < this->parsers.size(); i++)
		{
			auto token = this->parsers[i];
			switch (namespread)
			{
				case 1:
					if (token->pEnum != parserEnum::identifier)
						ERROR(("namespace�̖��O�����ʎq�ł͂���܂���" + getlinestring(this->program, token->sourcestartindex)).c_str());
					//BUG!!
					//if(!namesp.empty())namesp += "::";//�擪�ɂ�::���t��
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
					if (funcRead != 6)
					{
						if (namespread == 0)
						{
							funcStack.push(BlockStruct(sts::Empty, std::string()));
						}
						else
							funcStack.push(BlockStruct(sts::NameSpace, namesp));
					}/*else
					 func++;*/
					break;
				case parserEnum::blockend:
					if (funcStack.size() != 0)
					{
						if (funcStack.top().type == sts::NameSpace)
						{
							namesp.clear();
							auto cont = funcStack._Get_container();
							//if( funcStack.size()>=2)for(size_t i = funcStack.size() - 2;i>=0;i--)

							if (funcStack.size() >= 2)
#if _MSC_VER >=1800
							for (size_t i = 0; i < funcStack.size() - 1; i++)
#else
							for (size_t i = funcStack.size() - 1; i >= 0; i--)
#endif//for(size_t i = funcStack.size() - 1;i>=0;i--){
							{
								if (cont[i].type == sts::NameSpace)
								{
									namesp = cont[i].name;
									break;
								}
							}
						}
						if (funcStack.top().type == sts::Func) func--;
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
					if (token->pEnum == _class)
					{
						if (funcRead != 0)
							ERROR((getlinestring(this->program, token->sourcestartindex) + "�֐���͒��̃N���X").c_str());// WARNING("�֐���͒��̃N���X");
						if (funcStack.size() != 0 && funcStack.top().type == sts::Empty)
							ERROR((getlinestring(this->program, token->sourcestartindex) + "�l�X�g���ꂽ�N���X").c_str());
						classRead++;
					}
					break;
				case 1:
					if (token->pEnum != identifier)
						ERROR(("�N���X�̖��O�����ʎq�ł͂���܂���" + getlinestring(this->program, token->sourcestartindex)).c_str());
					className = namesp + *token->name;
					if (member == nullptr)member = new membertype_(); else delete member;
					if (staticmember == nullptr)staticmember = new membertype_(); else delete staticmember;
					classRead++;
					extendname = -1;
					break;
				case 2:
					if (token->pEnum == colon)
					{
						classRead = 5;
						break;
					}
					if (token->pEnum != blockstart)
						ERROR(("class " + className + "{�Ő錾����K�v������܂��B" + getlinestring(this->program, token->sourcestartindex)).c_str());
					classRead++;
					class_read_stack_index = funcStack.size();
					break;
				case 3:
					if (class_read_stack_index == funcStack.size())
					{
						if (token->pEnum == identifier)
						{
							/*
							if (*token->name == "return")
							{
							break;
							}*/
							classRead++;
						}

						if (token->pEnum == _static)
						{
							isstatic = true;
						}
					}
					if (class_read_stack_index > funcStack.size())
					if (token->pEnum == blockend)
					{
						auto classs = newClass(className, 0, member, this->runner, staticmember);
						if (extendname != -1)this->extendslist.push_back(std::pair<int, langClass>(extendname, classs));
						this->runner->variable.add(className, classs);
						member = nullptr;
						staticmember = nullptr;
						classRead = 0;
					}
					break;
				case 4:
					if (token->pEnum == identifier)
					{
						if (i + 1 < this->parsers.size() && this->parsers[i + 1]->pEnum == semicolon)
						{
							if (isstatic)
								staticmember->push_back(std::pair<std::string, langObject>(*token->name, NULLOBJECT));
							else
								member->push_back(std::pair<std::string, langObject>(*token->name, NULLOBJECT));
							isstatic = false;
							classRead = 3;
						}
						if (i + 1 < this->parsers.size() && this->parsers[i + 1]->pEnum == equal)
						{
							if (isstatic)
							{
								staticmember->push_back(std::pair<std::string, langObject>(*token->name, NULLOBJECT));
#ifdef CPP11
								staticmemberevals.push_back(std::tuple<int, std::string&, std::string>(i + 2, *token->name, className));
#else
								staticmemberevals.push_back(std::pair<int,std::pair<std::string&,std::string> >(i + 2,std::pair<std::string&,std::string>(*token->name,className)));
#endif
							}
							else
								std::cout << "������" << std::endl, member->push_back(std::pair<std::string, langObject>(*token->name, NULLOBJECT));
							isstatic = false;
							classRead = 3;
						}
						if (i + 1 < this->parsers.size() && this->parsers[i + 1]->pEnum == leftparent)
						{
							classRead = 3;
							//isstatic = false;
						}
					}
					else if (token->pEnum == leftparent)
					{
						funcRead = 2;
						funcName = "ctor";
						classRead = 3;
						isstatic = false;
					}
					classRead = 3;
					//isstatic = false;
					break;
					//extends
				case 5:
					extendname = i;//*token->name;
					classRead = 2;
					break;
			}
			switch (funcRead)
			{
				case 0://type
					if (token->pEnum == parserEnum::identifier)
					{
						/*if(*token->name == "return")
						{
						break;
						}*/
						funcRead++;
					}
					break;
				case 1://name
					if (token->pEnum == parserEnum::identifier)
					{
						funcRead++;
						funcName = *token->name;
					}
					else funcRead = 0;
					break;
				case 2://(
					if (token->pEnum == parserEnum::leftparent)funcRead++; else funcRead = 0;
					break;
				case 3://type 
					if (token->pEnum == parserEnum::identifier)funcRead++; else funcRead = 0;
					if (token->pEnum == parserEnum::rightparent)funcRead = 6;
					break;
				case 4://name
					if (token->pEnum == parserEnum::identifier)
					{
						if (argList == nullptr)argList = new std::vector<std::string>();
						argList->push_back(*token->name);
						funcRead++;
					}
					else funcRead = 0;
					break;
				case 5://, or )
					if (token->pEnum == parserEnum::comma)funcRead -= 2;
					else if (token->pEnum == parserEnum::rightparent)funcRead++;
					else funcRead = 0;
					break;
				case 6://{
					if (argList == nullptr)argList = new std::vector<std::string>();
					funcStack.push(BlockStruct(sts::Func, funcName));
#if _DEBUG
					if (lang::parserresult)
					{
						std::cout << std::endl;
						for (int j = 0; j < funcStack.size(); j++)
						{
							std::cout << " ";
						}
						std::cout << namesp + funcName;
					}
#endif
					funcRead = 0;
					if (classRead == 3)
					{
						if (isstatic)
						{
							staticmember->push_back(std::pair<std::string, langObject>(funcName, newFunction(funcName, argList, this->runner, i)));
						}
						else
							member->push_back(std::pair<std::string, langObject>(funcName, newFunction(funcName, argList, this->runner, i)));
					}
					else if (func <= 0)
					{
						this->runner->variable.add(namesp + funcName, newFunction(namesp + funcName, argList, this->runner, i));
					}
					else
						delete argList;//�Ƃ肠����
					argList = nullptr;//new std::vector<std::string>();
					func++;
					//this->runner->variable.add(*funcName,std::make_shared<langFunction>(*funcName,argList,this->runner));
					isstatic = false;
					break;
			}
		}
		if (parent != 0)
		{
			WARNING("��v���Ȃ�����(");
		}
		if (bracket != 0)
		{
			WARNING("��v���Ȃ�����[");
		}
		if (parserresult)std::cout << std::endl;
	}
	void parser::namespaceparse()
	{
		std::string namesp;
		std::stack<BlockStruct> funcStack;
		std::string empty;
		for (int i = 0; i < this->parsers.size(); i++)
		{
			auto token = this->parsers[i];
			parseObj* nextoken = nullptr;
			if (i + 1 < this->parsers.size()) nextoken = this->parsers[i + 1];
			switch (token->pEnum)
			{
				case parserEnum::_using:
					if (nextoken == nullptr)
					{
						ERROR("�s����using");
						break;
					}
					this->usings.push_back(*nextoken->name);
					i++;
					break;
					//odentifier
				case parserEnum::identifier:
					if (!DEFINEDSCPEVAR(this->runner, *token->name))
					{
						//�{��
						bool success = false;
						auto name = namesp + *token->name;
						if (DEFINEDSCPEVAR(this->runner, name))
						{
							delete token->name;
							token->name = new std::string(name);
							break;
						}
						else
						{
#ifdef CPP11
							for (auto i : this->usings)
							{
#else
							for( auto it = this->usings.begin(); it != this->usings.end(); ++it )
							{
								auto i = *it;
#endif
								//�{��
								auto name = i + "::" + *token->name;
								if (DEFINEDSCPEVAR(this->runner, name))
								{
									delete token->name;
									token->name = new std::string(name);
									success = true;
									break;
								}
							}
						}
						//if(!success)WARNINGS(1, "%s��������Ȃ�", token->name->c_str());
					}
					break;
				case parserEnum::_namespace:
					namesp = *nextoken->name + "::";
					break;
				case parserEnum::blockstart:
					if (1 < i)
					{
						auto prevtoken = this->parsers[i - 2];
						if (prevtoken->pEnum == parserEnum::_namespace)
						{
							funcStack.push(BlockStruct(sts::NameSpace, namesp));
						}
						else funcStack.push(BlockStruct(sts::Empty, empty));
					}
					else funcStack.push(BlockStruct(sts::Empty, empty));
					break;
				case parserEnum::blockend:
					if (funcStack.size() != 0)
					{
						if (funcStack.top().type == sts::NameSpace)
						{
							namesp.clear();
							auto cont = funcStack._Get_container();
							if (funcStack.size() >= 2)
#if _MSC_VER >=1800
							for (size_t i = 0; i < funcStack.size() - 1; i++)
#else
							for(size_t i = funcStack.size() - 1;i>=0;i--)
#endif
								//for(size_t i = 0;i<=funcStack.size() - 2;i++)
							{
								if (cont[i].type == sts::NameSpace)
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
		for (int i = 0; i < this->staticevals.size(); i++)
		{
			int j = this->staticevals[i];
			this->runner->eval(this->parsers[j]->ptr, j);
		}
		for (int i = 0; i < this->extendslist.size(); i++)
		{
			auto&& j = this->extendslist[i];
			j.second->base = (langClass)this->runner->variable[*this->parsers[j.first]->name];
			if (j.second->base == NULLOBJECT)
			{
				WARNINGS(0, "�p�����ɂȂ�%s�N���X�����݂��܂���B", this->parsers[j.first]->name->c_str())
			}
		}
#if CPP11
		foreach_(auto &&i in_ this->staticmemberevals)
		{
#else
		for( auto it = this->staticmemberevals.begin(); it != this->staticmemberevals.end(); ++it )
		{
			auto i = *it;
#endif
#if CPP11
			((langClass)this->runner->variable[std::get<2>(i)])->thisscope->variable.set(std::get<1>(i), this->runner->eval(NULLOBJECT, std::get<0>(i)));
#else
			((langClass)this->runner->variable[i.second.second])->thisscope->variable.set(i.second.first,this->runner->eval(NULLOBJECT,i.first));
#endif
		}
	}
	void parser::staticparse()
	{
		std::stack<BlockStruct> funcStack;
		std::string namesp;
		std::string varname;
		int read = 0, block = 0;
		bool isclass = false;
		std::string empty;
		for (int i = 0; i < this->parsers.size(); i++)
		{
			auto token = this->parsers[i];
			parseObj* nextoken = nullptr;
			if (i + 1 < this->parsers.size()) nextoken = this->parsers[i + 1];
			switch (read)
			{
				case 0: //static
					if (token->pEnum == parserEnum::_static)
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
					if (token->pEnum == parserEnum::equal)
					{
						read++;
					}
					else read = 0;
					break;
				case 3:  //exp1517754
					staticevals.push_back(i - 2);
					read = 0;
					break;
				case -1:
					read = -2;
					break;
				case -2:
					if (token->pEnum == parserEnum::blockstart)block++;
					if (token->pEnum == parserEnum::blockend)block--;
					if (block == 0) read = 0;
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
					if (1 < i)
					{
						auto prevtoken = this->parsers[i - 2];
						if (prevtoken->pEnum == parserEnum::_namespace)
						{
							funcStack.push(BlockStruct(sts::NameSpace, namesp));
						}
						else

						if (prevtoken->pEnum == parserEnum::_class)
						{
							funcStack.push(BlockStruct(sts::stsClass, *this->parsers[i - 1]->name));
						}
						else funcStack.push(BlockStruct(sts::Empty, empty));
					}
					else funcStack.push(BlockStruct(sts::Empty, empty));
					break;
				case parserEnum::blockend:
					if (funcStack.size() != 0)
					{
						if (funcStack.top().type == sts::NameSpace)
						{
							namesp.clear();
							auto cont = funcStack._Get_container();
							//if( funcStack.size() >= 2)for(size_t i = funcStack.size() - 2;i>=0;i--)

							if (funcStack.size() >= 2)
								//VS2013����d�l�ς���Ă���H
#if _MSC_VER >=1800
							for (size_t i = 0; i < funcStack.size() - 1; i++)
#else
							for (size_t i = funcStack.size() - 1; i >= 0; i--)
#endif
							{
								if (cont[i].type == sts::NameSpace)
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
		errcount = 0;
		int line = 0;
		lang::gc = nullptr;
		this->program = (input);
#if _DEBUG
		if (lang::parserresult) std::cout << "Now Parsing...";
#endif
		auto sts = parserStatus::None;
		auto iden = new std::string();
		bool shiftJis = false, ASCII = true;
		int startindex = 0;
		int blockComment = 0;
		bool isWChar = false;
		for (int i = 0; i <= input.size(); i++)
		{
			bool islast = i >= input.size() - 1;
			bool isfast = i == 0;
			auto chr = input[i];
			char nextchr = 0;
			char prevchr;
			if (!islast)nextchr = input[i + 1];
			if (!isfast)prevchr = input[i - 1];
			switch (sts)
			{
				case parserStatus::None:None :
					isWChar = false;
					switch (chr)
					{
						case '(':
							this->parsers.push_back(new parseObj(parserEnum::leftparent, new std::string("("), i, i));
							break;
						case ')':
							this->parsers.push_back(new parseObj(parserEnum::rightparent, new std::string(")"), i, i));
							break;
						case '[':
							this->parsers.push_back(new parseObj(parserEnum::leftbracket, new std::string("["), i, i));
							break;
						case ']':
							this->parsers.push_back(new parseObj(parserEnum::rightbracket, new std::string("]"), i, i));
							break;
						case '+':
							if (nextchr == '+')
								this->parsers.push_back(new parseObj(parserEnum::plusplus, new std::string("++"), i, i + 1)), i++;
							else
								this->parsers.push_back(new parseObj(parserEnum::plus, new std::string("+"), i, i));
							break;
						case '-':
							this->parsers.push_back(new parseObj(parserEnum::minus, new std::string("-"), i, i));
							break;
						case '*':
							this->parsers.push_back(new parseObj(parserEnum::multiply, new std::string("*"), i, i));
							break;
						case '%':
							this->parsers.push_back(new parseObj(parserEnum::modulo, new std::string("%"), i, i));
							break;
						case ',':
							this->parsers.push_back(new parseObj(parserEnum::comma, new std::string(","), i, i));
							break;
						case '.':
							this->parsers.push_back(new parseObj(parserEnum::dot, new std::string("."), i, i));
							break;
						case '=':if (nextchr == '=')
							this->parsers.push_back(new parseObj(parserEnum::equalequal, new std::string("=="), i, i + 1)), i++;
								 else this->parsers.push_back(new parseObj(parserEnum::equal, new std::string("="), i, i));
								 break;
						case ';':
							this->parsers.push_back(new parseObj(parserEnum::semicolon, new std::string(";"), i, i));
							break;
						case ':':
							this->parsers.push_back(new parseObj(parserEnum::colon, new std::string(":"), i, i));
							break;
						case '<':
							if (nextchr == '=')
								this->parsers.push_back(new parseObj(parserEnum::lessequal, new std::string("<="), i, i + 1)), i++;
							else if (nextchr == '<')
								this->parsers.push_back(new parseObj(parserEnum::leftshift, new std::string("<<"), i, i + 1)), i++;
							else this->parsers.push_back(new parseObj(parserEnum::less, new std::string("<"), i, i));
							break;
						case '>':
							if (nextchr == '=')
								this->parsers.push_back(new parseObj(parserEnum::greaterequal, new std::string(">="), i, i + 1)), i++;
							else if (nextchr == '>')
								this->parsers.push_back(new parseObj(parserEnum::rightshift, new std::string(">>"), i, i + 1)), i++;
							else
								this->parsers.push_back(new parseObj(parserEnum::greater, new std::string(">"), i, i));
							break;
						case '{':
							this->parsers.push_back(new parseObj(parserEnum::blockstart, new std::string("{"), i, i));
							break;
						case '}':
							this->parsers.push_back(new parseObj(parserEnum::blockend, new std::string("}"), i, i));
							break;
						case '"':
							startindex = i;
							sts = parserStatus::ReadStr;
							break;
						case 'L':
							if (nextchr != '\'') goto _default;
							isWChar = true;
							i++;
						case '\'':
							startindex = i;
							sts = parserStatus::ReadChar;
							break;
						case '/':
							if (nextchr == '/')
							{
								sts = parserStatus::ReadComment; i++;
							}
							else
							if (nextchr == '*')
							{
								sts = parserStatus::ReadBlockComment; i++; blockComment++;
							}
							else
							if (nextchr == '=')
								this->parsers.push_back(new parseObj(parserEnum::divisionequal, new std::string("/="), i, i + 1)), i++;
							else this->parsers.push_back(new parseObj(parserEnum::division, new std::string("/"), i, i));
							break;
						case '\n': line++;
						case '\r':case '\t':
						case ' ':break;
						default:_default :
							if (isNum(chr))
							{
								startindex = i;
								sts = parserStatus::ReadNum;
								goto ReadNum;
							}
							else
							if (isIden(chr, nextchr))
							{
								startindex = i;
								sts = parserStatus::ReadIden;
								goto ReadIden;
							}
							else if (shiftJis && isIdenShiftJIS(chr))
							{
								startindex = i;
								sts = parserStatus::ReadIden;
								goto ReadIden;
							}

							this->parsers.push_back(new parseObj(parserEnum::none, new std::string(input.substr(i, 1)), i, i));
							break;
					}
					break;
				case parserStatus::ReadIden:
				ReadIden :
					if (!isIden(chr, nextchr))
					{
						if (!(shiftJis && isIdenShiftJIS(chr)))
						if (!shiftJis || !isIdenShiftJIS1(prevchr))
						{
							int aho = aho_hash(iden->c_str());
							bool ok = false;
							//�O�����������r����͖̂��ʂ���������K���Ƀn�b�V���l���v�Z���čő�œ�񕪂ɂ��Ă݂�e�X�g
							switch (aho)
							{
								case HASHCLASS:
									if (*iden == "class"){ this->parsers.push_back(new parseObj(parserEnum::_class, iden, startindex, i - 1)); ok = true; }
									break;
								case HASHNEW:
									if (*iden == "for"){ this->parsers.push_back(new parseObj(parserEnum::_for, iden, startindex, i - 1)); ok = true; }
									if (*iden == "new"){ this->parsers.push_back(new parseObj(parserEnum::_new, iden, startindex, i - 1)); ok = true; }
									break;
								case HASHTHIS:
									if (*iden == "this"){ this->parsers.push_back(new parseObj(parserEnum::_this, iden, startindex, i - 1)); ok = true; }
									break;
								case HASHNAMESPACE:
									if (*iden == "namespace"){ this->parsers.push_back(new parseObj(parserEnum::_namespace, iden, startindex, i - 1)); ok = true; }
									break;
								case HASHUSING:
									if (*iden == "using"){ this->parsers.push_back(new parseObj(parserEnum::_using, iden, startindex, i - 1)); ok = true; }
									break;
								case HASHSTATIC:
									if (*iden == "static"){ this->parsers.push_back(new parseObj(parserEnum::_static, iden, startindex, i - 1)); ok = true; }
									break;
								case HASHTRUE:
									if (*iden == "true"){ this->parsers.push_back(new parseObj(parserEnum::_true, iden, startindex, i - 1, TRUEOBJECT)); ok = true; }
									break;
								case HASHFALSE:
									if (*iden == "false"){ this->parsers.push_back(new parseObj(parserEnum::_false, iden, startindex, i - 1, FALSEOBJECT)); ok = true; }
									break;
								case HASHNULL:
									if (*iden == "null"){ this->parsers.push_back(new parseObj(parserEnum::_null, iden, startindex, i - 1, NULLOBJECT)); ok = true; }
									break;
								case HASHBREAK:
									if (*iden == "break"){ this->parsers.push_back(new parseObj(parserEnum::_break, iden, startindex, i - 1)); ok = true; }
									break;
								case HASHCONTINUE:
									if (*iden == "continue"){ this->parsers.push_back(new parseObj(parserEnum::_continue, iden, startindex, i - 1)); ok = true; }
									break;
								case HASHELSE:
									if (*iden == "else"){ this->parsers.push_back(new parseObj(parserEnum::_else, iden, startindex, i - 1)); ok = true; }
									break;
								case HASHWHILE:
									if (*iden == "while"){ this->parsers.push_back(new parseObj(parserEnum::_while, iden, startindex, i - 1)); ok = true; }
									break;
								case HASHIF:
									if (*iden == "if"){ this->parsers.push_back(new parseObj(parserEnum::_if, iden, startindex, i - 1)); ok = true; }
									break;
								case HASHRETURN:
									if (*iden == "return"){ this->parsers.push_back(new parseObj(parserEnum::_return, iden, startindex, i - 1)); ok = true; }
									break;
								case HASHBASE:
									if (*iden == "base"){ this->parsers.push_back(new parseObj(parserEnum::base, iden, startindex, i - 1)); ok = true; }
									break;
								case HASHFOREACH:
									if (*iden == "foreach"){ this->parsers.push_back(new parseObj(parserEnum::_foreach, iden, startindex, i - 1)); ok = true; }
									break;
								case HASHIN:
									if (*iden == "in"){ this->parsers.push_back(new parseObj(parserEnum::_in, iden, startindex, i - 1)); ok = true; }
									break;
								case HASHIS:
									if (*iden == "is"){ this->parsers.push_back(new parseObj(parserEnum::_is, iden, startindex, i - 1)); ok = true; }
									else
									if (*iden == "as"){ this->parsers.push_back(new parseObj(parserEnum::_as, iden, startindex, i - 1)); ok = true; }
									break;
							}
							if (!ok) this->parsers.push_back(new parseObj(parserEnum::identifier, iden, startindex, i - 1));
							iden = new std::string();//!!!!�R�s�[����Ȃ��̂�new ����!!!!
							sts = parserStatus::None;
							goto None;
						}
					}
					//std::string chrs(chr);
					iden->append(input.substr(i, 1));
					break;
				case parserStatus::ReadNum:ReadNum :
					if (!isNum(chr))
					{
						if (chr == '.')
						{
							iden->append(input.substr(i, 1));
							sts = parserStatus::ReadDouble;
							break;
						}
						this->parsers.push_back(new parseObj(std::atoi(iden->c_str()), startindex, i - 1));
						iden->clear();//!!!!�R�s�[�����̂�clear ����!!!!
						sts = parserStatus::None;
						goto None;
					}
					//std::string chrs(chr);
					iden->append(input.substr(i, 1));
					break;
				case parserStatus::ReadDouble:
					if (!isNum(chr))
					{
						std::stringstream ss;
						double d;
						ss << *iden;
						ss >> d;
						this->parsers.push_back(new parseObj(d, startindex, i - 1));
						iden->clear();
						sts = parserStatus::None;
						goto None;
					}
					//std::string chrs(chr);
					iden->append(input.substr(i, 1));
					break;
				case parserStatus::ReadStr:ReadStr :
					if (chr == '"')
					{
						this->parsers.push_back(new parseObj(*iden, startindex, i));
						iden->clear();//!!!!�R�s�[�����̂�clear ����!!!!
						sts = parserStatus::None;
					}
					else
					{
						if (chr == '\\' && !(shiftJis && isIdenShiftJIS1(prevchr)))
						{
							sts = parserStatus::ReadEscapeString;
						}
						else
						if (chr == '\n' || chr == '\0')
						{
							WARNING("�����Ă��Ȃ�string");//throw langParseException("�����Ă��Ȃ�string");
						}
						else
							iden->append(input.substr(i, 1));
					}
					break;
				case parserStatus::ReadChar:ReadChar :
					if (chr == '\'')
					{
						if (iden->size() < 1 || iden->size() > 1)
						{
							if (iden->size() == 2)
							{
								if (!isWChar)
								{
									WARNINGS(1, "char��size��1�ł�WChar�ɕύX���܂��BL\'%s\'", iden->c_str());
									isWChar = true;
								}
							}
							else
								WARNINGS(0, "char��size��1�ł�%s", iden->c_str());
						}
						if (isWChar)
						{
							int size = iden->size();
							const char* wStrC = iden->c_str();
							//�ϊ�������i�[�o�b�t�@
							wchar wStrW[2];
							size_t wLen = 0;
							errno_t err = 0;
							//���P�[���w��
							setlocale(LC_ALL, "japanese");
							//�ϊ�
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
						if (chr == '\\' && !(shiftJis && isIdenShiftJIS1(prevchr)))
						{
							sts = parserStatus::ReadEscapeChar;
						}
						else
						if (chr == '\n' || chr == '\0')
						{
							WARNING("�����Ă��Ȃ�char");//throw langParseException("�����Ă��Ȃ�string");
						}
						else
							iden->append(input.substr(i, 1));
					}
					break;
				case parserStatus::ReadComment:
					if (chr == '\n' || chr == '\0') sts = parserStatus::None;
					break;
				case parserStatus::ReadBlockComment:
					if (chr == '*' && nextchr == '/') { blockComment--; i++; }
					if (chr == '/' && nextchr == '*') { blockComment++; i++; }
					if (!blockComment) sts = parserStatus::None;
					break;
				case parserStatus::ReadEscapeString:
					switch (chr)
					{
						case 'n':
							*iden += '\n';
							break;
						case 't':
							*iden += '\t';
							break;
						case '\"':
							*iden += chr;
							break;
						case '\'':
							*iden += chr;
							break;
						default:
							WARNING((std::string("�F���ł��Ȃ��G�X�P�[�v�V�[�P���X") + chr).c_str(), 1);
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
							*iden += '\t';
							break;
						case '\"':
							*iden += chr;
							break;
						case '\'':
							*iden += chr;
							break;
						default:
							WARNING((std::string("�F���ł��Ȃ��G�X�P�[�v�V�[�P���X") + chr).c_str(), 1);
							break;
					}
					sts = parserStatus::ReadChar;
					break;
				default:
					throw langParseException("WHAT???");
					break;
			}
#if _DEBUG
			if (lang::parserresult) std::cout << chr;
#endif
		}
		if (iden->empty())delete iden;//�g���ĂȂ�����delete
		lang::plugin::dagger(this->parsers);
		this->function();
		this->staticparse();
		this->namespaceparse();
#if _DEBUG
		if (lang::parserresult)std::cout << "Done" << std::endl;
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
		catch (...)
		{
		}
	}
	langObject parser::Run()
	{
		if (errcount > 0)
		{
			std::cerr << "ERROR" << errcount << std::endl;
			return NULLOBJECT;
		}
		return this->runner->run();
	}
}
