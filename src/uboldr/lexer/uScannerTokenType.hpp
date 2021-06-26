#pragma once

#include <stdint.h>

enum class uScannerTokenType : int8_t
{
	Unknown = -1,
	IntLiteral = 0,
	FloatLiteral = 1,
	StringLiteral = 2,
	Identifier = 3,
	Keyword = 4,
	Symbol = 5,
	EoF = 6,
};

