#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <codecvt>
#include <cstdlib>
#include <ios>
#include <iosfwd>

namespace UTF8
{
namespace File
{
	bool Exists(std::wstring path);
	int64_t Size(std::wstring path);
	int64_t Size(std::string path);
}
}
