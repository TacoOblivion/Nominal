
#include "FileStream.h"
#include "file/File.h"
#include "../basic/Conversions.h"


UTF8::FileStream::FileStream(const std::string filePath)
{
	this->_path = StrToWStr(filePath);
	_codeConverter = new std::codecvt_utf8<wchar_t>;
}

UTF8::FileStream::FileStream(const std::wstring filePath)
{
	this->_path = filePath;
	_codeConverter = new std::codecvt_utf8<wchar_t>;
}


UTF8::FileStream::~FileStream()
{
	//delete _codeConverter;
}

UTF8::FileStream& UTF8::FileStream::Open(UTF8::OpenMode mode = UTF8::OpenMode::DefaultRead)
{
	if (((int32_t)mode & (int32_t)UTF8::OpenMode::Read) == (int32_t)UTF8::OpenMode::Read)
		this->_size = File::Size(_path);
	
	_file.open(_path, (std::ios_base::openmode)mode);
	_file.imbue(std::locale(_file.getloc(), _codeConverter));
	return *this;
}

void UTF8::FileStream::Close()
{
	_file.close();
}

wchar_t UTF8::FileStream::ReadNext()
{
	return _file.get();
}

wchar_t* UTF8::FileStream::ReadAll()
{
	wchar_t* buffer = new wchar_t[this->_size + 1];

	for (int64_t i = 0; i < this->_size; ++i)
		buffer[i] = this->_file.get();

	buffer[this->_size] = L'\0';

	return buffer;
}
