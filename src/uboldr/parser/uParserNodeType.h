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

enum class uParserNodeType : int32_t
{
	Unknown = 0,

	// Arithmetic Operations (1-10)
	Add = 1,					// a + b	e.g. 1 + 1		= 1
	Plus = 2,					// +a	e.g. +1			= 1
	Subtract = 3,				// a - b	e.g. 1 - 1		= 0
	Minus = 4,					// -a	e.g. -1			= -1
	Negate = 4,
	Multiply = 5,				// a * b	e.g. 1 * 1		= 1
	Divide = 6,					// a / b 	e.g. 1 / 1		= 1
	Modulus = 7,				// a % b	e.g. 5 % 3		= 2
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
	PreIncrement = 8,			// ++a	e.g. ++a
	PostIncrement = 9,			// a++	e.g. a++
	PreDecrement = 10,			// --a	e.g. --a
	PostDecrement = 11,			// a--	e.g. a--
	ArithmeticRightShift = 12,	// a >> b	e.g. -1 >> 1	
	// 

	// Bool Operations (16-20)
	BoolAnd = 16,				// a && b
	BoolOr = 17,				// a || b
	BoolNot = 18,				// !a
	BoolXor = 19,				// a xor b
	BoolImplication = 20,		// a impl b

	// Comparison Operations (21-30)
	EqualTo = 21,				// a == b
	NotEqualTo = 22,			// a != b
	IdenticalTo = 23,			// a === b
	NotIdenticalTo = 24,		// a !== b
	GreaterThan = 25,			// a > b
	GreaterThanEqual = 26,		// a >= b
	//NotGreaterThan = 28,		// a !> b same as a <= b
	//NotGreaterThanEqual = 27,	// a !>= b same as a < b
	LessThan = 27,				// a < b
	LessThanEqual = 28,			// a <= b
	//NotLessThan = 26,			// a !< b same as a >= b
	//NotLessThanEqual = 25,	// a !<= b same as a > b

	// Bitwise Operations (31-40)
	BitwiseAnd = 31,			// a & b
	BitwiseOr = 32,				// a | b
	BitwiseNot = 33,			// ~a
	BitwiseXor = 34,			// a ^ b
	BitwiseImplication = 35,	// a -> b
	BitwiseLeftShift = 36,		// a << b
	BitwiseRightShift = 37,		// a >>> b

	// Other
	Reference = 40,				// &a

	// Parser Groups
	Block = 45,
	Integer = 46,
	Float = 47,
	Identifier = 48,
	String = 49,
	Null = 50,

	// Precedence Top Precedence
	StatementList = 50,

	// Precedence - 1
	IfStatement = 51,
	WhileStatement = 52,
	ForStatement = 53,
	DeleteStatement = 54,
	Assignment = 59,

	IfBlock = 60,
	TernaryIfBlock = 61,
	ForStatementListInitializer = 62,
	ForStatementListCondition = 63,
	ForStatementListLoopCounter = 64,

	// Precedence - 2
	// ExprBoolOr

	// Precedence - 3
	// ExprBoolAnd

	// Precedence - 4
	// ExprBitwiseOr

	// Precedence - 5
	// ExprBitwiseXor

	// Precedence - 6
	// ExprBitwiseAnd
	
	// Precedence - 7
	// ExprEquality

	// Precedence - 8
	// ExprRelational

	// Precedence - 9
	// ExprBitShift

	// Precedence - 10
	// ExprMathAddSub

	// Precedence - 11
	// ExprMathMulDivMod

	// Precedence - 12
	// ExprPreUnary

	// Precedence - 13
	// ExprPostUnary

	// Precedence - 14
	// ExprCallAndMemberAccess
	FnCall = 80,
	BracketAccess = 81,
	MemberAccess = 82,
};
