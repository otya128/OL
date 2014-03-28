#pragma once

#include"stdafx.h"

#ifndef PARSERENUM_LENGTH
#define PARSERENUM_LENGTH 48
namespace lang
{
	enum parserEnum
	{
		identifier,//���ʎq
		num,//���l
		doublequote,//
		str,//������
		leftparent,//(
		rightparent,//)
		comma,//,
		plus,//+
		minus,//-
		multiply,//*
		equalequal,//==
		semicolon,//;
		blockstart,//{
		blockend,//}
		plusplus,//++
		minusminus,//--
		greater,//>
		less,//<
		//equal�n greaterequal~xorequal�܂�
		greaterequal,//>=
		lessequal,//<=
		modulo,//%
		equal,//=
		plusequal,//+=
		minusequal,//-=
		multiplyequal,//*=
		divisionequal,// /=
		moduloequal,//%=
		leftshiftequal,//<<=
		rightshiftequal,//>>=
		andequal,//&=
		orequal,//|=
		xorequal,//^=
		dot,//.
		division,///
		leftbracket,//[
		rightbracket,//]
		debbug_stop,//$
		//�ǉ����Z�q
		_or,//|
		oror,//||
		_and,//&
		andand,//&&
		_xor,//^
		notequal,//!
		_not,//~
		notnot,//!LogicalNot
		leftshift,//<<
		rightshift,//>>
		chr,//single str
		none,
		_class,
		_new,
		_this,
		_namespace,
		_using,
		_static,
		//�萔
		_true,
		_false,
		_null,
		_break,
		_continue,
		_else,
		_while,
		_for,
		_if,
		_return,
		base,
		colon,
		_foreach,
		_in,
		_is,
		_as,
		lambda,
		conditional,//?
		_catch,
		_throw,
	};
}
#endif
