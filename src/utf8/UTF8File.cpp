
#include "UTF8File.h"
#include "../basic/Conversions.h"

/*UTF8File::UTF8File(const char* filePath)
{
	this->_path = filePath;
	_codeConverter = new std::codecvt_utf8<wchar_t>;
}*/

UTF8File::UTF8File(const std::string filePath)
{
	this->_path = StrToWStr(filePath);
	_codeConverter = new std::codecvt_utf8<wchar_t>;
}

UTF8File::UTF8File(const std::wstring filePath)
{
	this->_path = filePath;
	_codeConverter = new std::codecvt_utf8<wchar_t>;
}


UTF8File::~UTF8File()
{
	//delete _codeConverter;
}

UTF8File& UTF8File::Open(UTF8OpenMode mode = UTF8OpenMode::DefaultRead)
{
	if (((int32_t)mode & (int32_t)UTF8OpenMode::Read) == (int32_t)UTF8OpenMode::Read)
		this->_bufferSize = this->_GetFileBufferSize();

	_file.open(_path, (std::ios_base::openmode)mode);
	_file.imbue(std::locale(_file.getloc(), _codeConverter));
	return *this;
}

void UTF8File::Close()
{
	_file.close();
}

wchar_t* UTF8File::ReadAll()
{
	wchar_t* buffer = new wchar_t[this->_bufferSize + 1];

	for (int64_t i = 0; i < this->_bufferSize; ++i)
	{
		buffer[i] = this->_file.get();

		if ((buffer[i] & 0xFF) == 0)
			buffer[i] = buffer[i] >> 8;
	}

	buffer[this->_bufferSize] = L'\0';

	return buffer;
}

int64_t UTF8File::_GetFileBufferSize()
{
	std::wifstream in(_path, std::wifstream::ate | std::wifstream::binary);
	return (int64_t)in.tellg() - 1LL;
	/*_file.open(_path, std::ios_base::in);
	_file.imbue(std::locale(_file.getloc(), _codeConverter));
	_file.seekg(0, (std::ios_base::seekdir)0); // ios::beg keeps causing red underlines, so 0 for the 2nd argument
	
	int64_t i = -1;
	while (!_file.eof())
	{
		++i;
		_file.get();
	}

	_file.close();
	//Serial.println("");
	//Serial.print("WSize: ");
	//Serial.println((int32_t)i);
	return i;*/
}
