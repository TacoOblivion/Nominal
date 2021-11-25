#pragma once

#include "../../basic/Types.h"
#include "LexerTokenType.hpp"

// fwrd decl
struct AbstractLexerToken;
template <typename T>
struct LexerToken;

struct AbstractLexerToken : public object_t
{
	int64_t LineIndex = -1;
	int32_t CharIndex = -1;
	uScannerTokenType Type = uScannerTokenType::Unknown;

	AbstractLexerToken() {}
	virtual ~AbstractLexerToken() {}

	template <typename DT>
	DT GetData() { return ((LexerToken<DT>*)this)->Data; }
};

template <typename T>
struct LexerToken : public AbstractLexerToken
{
	T Data;

	LexerToken(T data) : Data(data) {}
	virtual ~LexerToken() {}

	T GetData();
};

template<typename T>
inline T LexerToken<T>::GetData()
{
	return Data;
}

/*template <typename V>
struct uStringToken : public LexerToken<std::string, V>
{
};*/

/*template <typename V>
struct uIntToken : public LexerToken<int64_t, V>
{
};

template <typename V>
struct uFloatToken : public LexerToken<float64_t, V>
{
};

template <typename V>
struct uSymbolToken : public LexerToken<uTokenType, V>
{
};*/


