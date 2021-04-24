#pragma once

#include <xstring>

bool WStrEqualsStr(std::wstring wstr, std::string str)
{
	if (wstr.length() != str.length())
		return false;

	auto length = wstr.length();

	for (int64_t i = 0; i < length; ++i)
		if ((char)wstr[i] != str[i])
			return false;

	return true;
}
