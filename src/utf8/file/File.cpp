#include "File.h"

bool UTF8::File::Exists(std::wstring path)
{
	//TODO: Implement for Arduino

	return false;
}

int64_t UTF8::File::Size(std::wstring path)
{
	std::wifstream file(path, std::wifstream::ate | std::wifstream::binary);
	return (int64_t)file.tellg() - 1LL;
}

int64_t UTF8::File::Size(std::string path)
{
	std::wifstream file(path, std::wifstream::ate | std::wifstream::binary);
	return (int64_t)file.tellg() - 1LL;
}
