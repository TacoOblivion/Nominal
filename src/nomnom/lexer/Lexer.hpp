#pragma once

#ifndef _USCANNER_h
#define _USCANNER_h

#include "../../basic/Conversions.h"
#include "../../utf8/FileStream.h"
#include "../../collections/LinkedList.h"
#include "LexerToken.hpp"
#include "LexerTokenType.hpp"
#include "LexerSymbolType.hpp"
#include <xstring>

class uScanner
{
	std::wstring _path;
	UTF8::FileStream _file;
	int64_t _pos;
	int64_t _line = 0;
	//int32_t scanChar = 0;
	int64_t _lastNewLine = 0;
	wchar_t* _contents;
	const static int32_t KeywordCount = 5 * 10;
	const std::string Keywords[KeywordCount]
	{
		"int8", "int16", "int32", "int64", "int128",
		"uint8", "uint16", "uint32", "uint64", "uint128",
		"float16", "float32", "float64", "float128", "string",
		"complex64", "complex128", "", "", "",
		"icomplex16", "icomplex32", "icomplex64", "icomplex128", "",
		"var", "object", "const", "static", "bool",
		"if", "else", "while", "for", "loop",
		"fn", "", "", "", "",
		"partial", "class", "interface", "", "",
		"__VERSION__", "__FILEPATH__", "", "", "",
	};

public:
	uScanner(std::string path);
	uScanner(std::wstring path);
	LinkedList<AbstractLexerToken*>* Scan();
	wchar_t Peek(int64_t pos = 0);
	wchar_t GetChar();
	wchar_t GetNextChar();
	inline bool IsUpperAlpha(wchar_t chr);
	inline bool IsLowerAlpha(wchar_t chr);
	inline bool IsAlpha(wchar_t chr);
	inline bool IsNumeric(wchar_t chr);
	inline bool IsAlphanumeric(wchar_t chr);
	inline bool IsKeyword(std::wstring wstr);
	std::string ReadString(int64_t startPos, int64_t length);
	std::wstring ReadWString(int64_t startPos, int64_t length);

	static std::string TokenTypeToString(uScannerTokenType type);
	static std::string SymbolToString(uSymbolType symbol);
	
	//LexerToken* CreateToken(uTokenType type);
	//template <typename T>
	//LexerToken* CreateToken(uScannerTokenType type, T data);
	template <typename T>
	LexerToken<T>* CreateToken(uScannerTokenType scannerTokenType, T data);
};

template<typename T>
inline LexerToken<T>* uScanner::CreateToken(uScannerTokenType scannerTokenType, T data)
{
	auto token = new LexerToken<T>(data);
	token->Type = scannerTokenType;
	token->LineIndex = _line;
	token->CharIndex = _pos - _lastNewLine;

	return token;
};

#endif // _USCANNER_h
