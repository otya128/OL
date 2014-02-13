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
        //�ǉ����Z�q
        multiplyequal,//*=
        divisionequal,// /=
        moduloequal,//%=
        or,//|
        oror,//||
        and,//&
        andand,//&&
        xor,//^
        notequal,//!
        not,//~
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
    };
}
#endif
