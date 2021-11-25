#pragma once

#include <stdint.h>

enum class uSymbolType : int8_t
{
	Unknown,

	// Arithmetic Operations
	Plus,					// +
	Minus,					// -
	Asterisk,				// *
	Divide,					// /
	Percent,				// %
	Increment,				// ++
	Decrement,				// --
	ArithmeticRightShift,	// >>

	// Bool Operations
	BoolAnd,			// &&
	BoolOr,				// ||
	BoolNot,			// !

	// Comparison Operations
	EqualTo,			// ==
	NotEqualTo,			// !=
	IdenticalTo,		// ===
	NotIdenticalTo,		// !==
	GreaterThan,		// >
	GreaterThanEqual,	// >=
	// !> becomes <=
	// !>= becomes <
	LessThan,			// <
	LessThanEqual,		// <=
	// !< becomes >=
	// !<= becomes >
	NullCoalescing,		// ??

	// Bitwise Operations
	Ampersand,			// &
	Pipe,				// |
	Tilde,				// ~
	Circumflex,			// ^
	BitwiseImplication,	// ->
	BitwiseLeftShift,	// <<
	BitwiseRightShift,	// >>>

	// Assignment
	Equals,						// =
	PlusEquals,					// +=
	MinusEquals,				// -=
	TimesEquals,				// *=
	DividedByEquals,			// /=
	ModuloEquals,				// %=
	ArithmeticRightShiftEquals,	// >>=
	BitwiseAndEquals,			// &=
	BitwiseOrEquals,			// |=
	BitwiseNotEquals,			// ~=
	BitwiseXorEquals,			// ^=
	BitwiseImplicationEquals,	// ->=
	BitwiseLeftShiftEquals,		// <<=
	BitwiseRightShiftEquals,	// >>>=
	NullCoalescingEquals,		// ??=

	// Grouping
	ParanLeft,						// (
	ParanRight,						// )
	SquareBracketLeft,				// [
	SquareBracketRight,				// ]
	CurlyBraceLeft,					// {
	CurlyBraceRight,				// }
	NullCoalescingSquareBracket,	// ?[

	// Access
	Dot,					// .
	NullCoalescingAccess,	// ?.
	
	// Range
	Range,				// ..

	// Preprocessor
	Sharp,				// #

	// Special Directive
	At,					// @

	// Ternary & Other
	QuestionMark,		// ?
	Colon,				// :
	SemiColon,			// ;
	AutoType,			// :=
};
