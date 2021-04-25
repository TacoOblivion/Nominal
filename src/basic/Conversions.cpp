
#include <xstring>

std::string WStrToStr(std::wstring wstr)
{
	std::string str;
	str.reserve(wstr.size());
	auto len = wstr.length();

	for (int64_t i = 0; i < len; ++i)
		str.push_back(wstr[i]);

	return str;
}

std::wstring StrToWStr(std::string str)
{
	std::wstring wstr;
	wstr.reserve(str.size());
	auto len = str.length();

	for (int64_t i = 0; i < len; ++i)
		wstr.push_back(str[i]);

	return wstr;
}
