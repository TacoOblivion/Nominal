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
#include "UTF8OpenMode.hpp"

class UTF8File
{
	std::string _path;
	int64_t _bufferSize = 0;
	std::wfstream _file;
	std::codecvt_utf8<wchar_t>* _codeConverter;

	void _Open();
	int64_t _GetFileBufferSize();
public:
	UTF8File(const char* filePath);
	UTF8File(const std::string& filePath);
	~UTF8File();
	UTF8File& Open(UTF8OpenMode mode);
	void Close();

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

	int64_t BufferSize() { return _bufferSize; }
};

#endif

