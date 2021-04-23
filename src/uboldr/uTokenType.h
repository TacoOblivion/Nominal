#pragma once

#include <stdint.h>
//#include <string>
//#include <fstream>
//#include <locale>
//#include <codecvt>
//#include <cstdlib>

// (+2)a;
// becomes...
// a += 2;
// push a;

// a(+2) + 10;
// becomes...
// push a + 10;
// a += 2;

// a(* a + 5) - b;
// becomes...
// push a - b;
// a *= a + 5

enum class uParserTokenType : int32_t
{
	Unknown = 0,

	// Arithmetic Operations (1-10)
	Add = 1,					// +	e.g. 1 + 1		= 1
	Plus = 2,					// +	e.g. +1			= 1
	Subtract = 3,				// -	e.g. 1 - 1		= 0
	Minus = 4,					// -	e.g. -1			= -1
	Multiply = 5,				// *	e.g. 1 * 1		= 1
	Divide = 6,					// /	e.g. 1 / 1		= 1
	Modulus = 7,				// %	e.g. 5 % 3		= 2
								//		     -5 % 0..3	= 1		diff = 3-0 = 3
								//		     9 % 4..7	= 4		diff = 7-4 = 3
								//		     -5 % -3..0	= -2	diff = 0-(-3) = 3
								// note that ranges will automatically flip their lhs and rhs if the rhs < lhs
								//		     if lhs < 0:
								//				m = (((lhs / diff) - 1) * diff) * -1
								//				r = lhs_min + ((lhs + m) % diff)
								//			 else:
								//				r = lhs_min + (lhs % diff)
								//		     
								//				m = (((-5 / 3) - 1) * 3) * -1 = ((-1 - 1) * 3) * -1 = (-2 * 3) * -1 = -6 * -1 = 6
								// -5 % 0..3	r = -3 + ((-5 + 6) % 3) = -3 + (1 % 3) = -3 + 1 = -2
								// -5 % -3..0	r = 0 + ((-5 + 6) % 3) = 0 + (1 % 3) = 0 + 1 = 1
								//				
								// 9 % 4..7		r = 4 + (9 % 3) = 4 + 0 = 4
	//Increment = 8,				// ++
	PreIncrement = 8,			// ++	e.g. ++a
	PostIncrement = 9,			// ++	e.g. a++
	//Decrement = 10,				// --
	PreDecrement = 10,			// --	e.g. --a
	PostDecrement = 11,			// --	e.g. a--
	ArithmeticRightShift = 12,	// >>>	e.g. -1 >>> 1	= 0x7FFFFFFF
	// 

	// Bool Operations (16-20)
	BoolAnd = 16,				// &&
	BoolOr = 17,				// ||
	BoolNot = 18,				// !
	BoolXor = 19,				// xor
	BoolImplication = 20,		// impl

	// Comparison Operations (21-30)
	EqualTo = 21,				// ==
	NotEqualTo = 22,			// !=
	IdenticalTo = 23,			// ===
	NotIdenticalTo = 24,		// !==
	GreaterThan = 25,			// >
	GreaterThanEqual = 26,		// >=
	NotGreaterThan = 28,		// !> same as <=
	NotGreaterThanEqual = 27,	// !>= same as <
	LessThan = 27,				// <
	LessThanEqual = 28,			// <=
	NotLessThan = 26,			// !< same as >=
	NotLessThanEqual = 25,		// !<= same as >

	// Bitwise Operations (31-40)
	BitwiseAnd = 31,			// &
	BitwiseOr = 32,				// |
	BitwiseNot = 33,			// ~
	BitwiseXor = 34,			// ^
	BitwiseImplication = 35,	// ->
	BitwiseLeftShift = 36,		// <<
	BitwiseRightShift = 37,		// >>

	// ...
	Assignment = 100,
};
