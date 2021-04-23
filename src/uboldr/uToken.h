#pragma once

#include "../basic/Types.h"
#include "uTokenType.h"

template <typename V>
struct uAbstractToken : public object_t
{
	V LineIndex = 0;
	V CharIndex = 0;
	uTokenType Type = uTokenType::Unknown;

	uAbstractToken() {}
	virtual ~uAbstractToken() {};
};

template <typename T, typename V>
struct uToken : public uAbstractToken<V>
{
	T Data;

	uToken() {}
	virtual ~uToken() {};
};

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


