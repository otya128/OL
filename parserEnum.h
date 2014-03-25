#pragma once

#include"stdafx.h"

#ifndef PARSERENUM_LENGTH
#define PARSERENUM_LENGTH 48
namespace lang
{
	enum parserEnum
	{
		identifier,//éØï éq
		num,//êîíl
		doublequote,//
		str,//ï∂éöóÒ
		leftparent,//(
		rightparent,//)
		comma,//,
		plus,//+
		minus,//-
		multiply,//*
		equal,//=
		equalequal,//==
		semicolon,//;
		blockstart,//{
		blockend,//}
		plusplus,//++
		minusminus,//--
		greater,//>
		less,//<
		greaterequal,//>=
		lessequal,//<=
		modulo,//%
		plusequal,//+=
		minusequal,//-=
		dot,//.
		division,///
		leftbracket,//[
		rightbracket,//]
		debbug_stop,//$
		//í«â¡ââéZéq
		multiplyequal,//*=
		divisionequal,// /=
		moduloequal,//%=
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
		leftshiftequal,//<<=
		rightshiftequal,//>>=
		andequal,//&=
		orequal,//|=
		xorequal,//^=
		chr,//single str
		none,
		_class,
		_new,
		_this,
		_namespace,
		_using,
		_static,
		//íËêî
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
	};
}
#endif
