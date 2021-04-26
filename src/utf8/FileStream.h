// UTF8File.h
#pragma once

#ifndef _UTF8FILE_h
#define _UTF8FILE_h

#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <codecvt>
#include <cstdlib>
#include <ios>
#include <iosfwd>
//#include <bits/ios_base.h>
#include "OpenMode.h"

namespace UTF8
{

class FileStream
{
	std::wstring _path;
	int64_t _size = 0;
	std::wfstream _file;
	std::codecvt_utf8<wchar_t>* _codeConverter;

	void _Open();
public:
	//UTF8File(const char* filePath);
	FileStream(const std::string filePath);
	FileStream(const std::wstring filePath);
	~FileStream();
	FileStream& Open(UTF8::OpenMode mode);
	void Close();

	wchar_t ReadNext();
	wchar_t* ReadAll();
	//wchar_t* GetString(int64_t startPos, int64_t length);

	void Write(const char* data)
	{
		_file << data;
	}

	void Write(const wchar_t* data)
	{
		_file << data;
	}

	int64_t Size() { return _size; }
};

}

#endif
