#pragma once

#include "../../basic/Types.h"
#include "uScannerTokenType.h"

// fwrd decl
struct uAbstractToken;
template <typename T>
struct uToken;

struct uAbstractToken : public object_t
{
	int64_t LineIndex = -1;
	int32_t CharIndex = -1;
	uScannerTokenType Type = uScannerTokenType::Unknown;

	uAbstractToken() {}
	virtual ~uAbstractToken() {};

	template <typename DT>
	DT GetData() { return ((uToken<DT>*)this)->Data; }
};

template <typename T>
struct uToken : public uAbstractToken
{
	T Data;

	uToken(T data) : Data(data) {}
	virtual ~uToken() {};

	T GetData();
};

template<typename T>
inline T uToken<T>::GetData()
{
	return Data;
}

/*template <typename V>
struct uStringToken : public uToken<std::string, V>
{
};*/

/*template <typename V>
struct uIntToken : public uToken<int64_t, V>
{
};

template <typename V>
struct uFloatToken : public uToken<float64_t, V>
{
};

template <typename V>
struct uSymbolToken : public uToken<uTokenType, V>
{
};*/


