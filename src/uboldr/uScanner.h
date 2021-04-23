#pragma once

#ifndef _USCANNER_h
#define _USCANNER_h

#include "../utf8/UTF8File.hpp"
#include "../collections/LinkedList.h"
#include "uToken.h"
#include "uScannerTokenType.h"

class uScanner
{
	std::string _path;
	UTF8File _file;
	int32_t _pos;
	wchar_t* _contents;
	const static int32_t KeywordCount = 5 * 5;
	const std::string Keywords[KeywordCount]{
		"int8", "int16", "int32", "int64", "int128",
		"uint8", "uint16", "uint32", "uint64", "uint128",
		"float16", "float32", "float64", "float128", "string",
		"var", "object", "", "", "",
		"if", "else", "while", "for", "loop"
	};

public:
	uScanner(std::string path);
	LinkedList<object_t*>* Scan();
	wchar_t Peek(int64_t pos = 0);
	wchar_t GetChar();
	wchar_t GetNextChar();
	inline bool IsUpperAlpha(wchar_t chr);
	inline bool IsLowerAlpha(wchar_t chr);
	inline bool IsAlpha(wchar_t chr);
	inline bool IsNumeric(wchar_t chr);
	inline bool IsAlphanumeric(wchar_t chr);
	std::string ReadString(int64_t startPos, int64_t length);
	std::wstring ReadWString(int64_t startPos, int64_t length);
	
	//uToken* CreateToken(uTokenType type);
	//template <typename T>
	//uToken* CreateToken(uScannerTokenType type, T data);
	template <typename T>
	uToken<T, void>* CreateToken(uScannerTokenType scannerTokenType, T data);
};

template<typename T>
inline uToken<T, void>* uScanner::CreateToken(uScannerTokenType scannerTokenType, T data)
{
	auto token = new uToken<T, void>();
	token->type = scannerTokenType;
	token->data = data;

	return token;
};

#endif // _USCANNER_h
