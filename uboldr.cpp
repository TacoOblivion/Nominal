// 
//

/*
#include <iostream>
#include <string>
#include <fstream>
#include <locale>
#include <codecvt>
#include <cstdlib>
*/

//#include "src/utf8/UTF8File.h"
#include "src/collections/LinkedList.h"
//#include "src/uboldr/uScanner.h"
#include "src/uboldr/uToken.h"
#include <iostream>

using namespace std;

// for (var i given 0..12)
// loop (var i from 0..12)
// loop (var i from 0..12; ++20 i)
// loop (var i from -12..0; --2 i)
// loop (var i from 0..12; (++20)i)
// loop (var i from 0..12; ++20 for i)
// loop (var i from 0..12; ++i:20)
// loop (n
// ?.	when not null, access property of variable
// ??	when left hand side not null, set to lhs, else set to rhs
// &	set as reference to another var(basically an addr pointer)
// ..	range e.g.number == 10..20
// 



// reserved keywords...
// class, fn, function, extends, implements, interface, and, or, xor, if, else, while, for, loop, repeat, alias, yield
// reserved types
// int8, uint8, int16, uint16, int32, uint32, int64, uint64, string, array, float, double, char8, char16



int main()
{
	//std::locale::global(std::locale(std::locale::empty(), new std::codecvt_utf8<char32_t>));
	//std::locale::global(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
	//wifstream file;
	//file.open(L"test.ubr");
	//auto fileSize = GetFileSizeS(file);
	//wchar_t* buffer = (wchar_t*)calloc((int64_t)fileSize, sizeof(wchar_t));

	//cout << "Size1: " << ((int64_t)fileSize) << endl;
	//cout << "Size2: " << ((int64_t)GetFileSize(file)) << endl;
	//
	//int i = 0;
	//while (!file.eof())
	//{
	//	buffer[i++] = file.get();
	//	//wstring line;
	//	//std::getline(file, line);
	//	if (buffer[i - 1] == L'に')
	//		cout << "Found It!" << endl << "Pos: " << (i - 1) << endl;
	//	cout << (char)buffer[i - 1] << endl;
	//}

	//file.close();

	/*
	UTF8File file("test.ubr");
	file.Open(UTF8OpenMode::Read);
	auto buffer = file.ReadAll();
	
	int32_t i = 0;
	int32_t segStart = 0;
	int32_t segEnd = 0;
	wchar_t symbolBuffer[5];
	while (buffer[i] != L'\0')
	{
		wchar_t chr = buffer[i++];
		
		std::cout << (char)chr;
	}
	*/
	//int64_t* stack = (int64_t*)calloc(512, sizeof(int64_t));
	
	auto test0 = new uToken<std::string, int32_t>();
	test0->Data = "Hello World";
	void* test1 = (void*)(test0);
	uToken<std::string, int32_t>* test2 = (uToken<std::string, int32_t>*)test1;

	cout << test2->Data;
	
	getchar();
}

//streampos GetFileSize(wifstream& file)
//{
//	auto pos = file.tellg();
//	file.seekg(0, ios::beg);
//	auto begin = file.tellg();
//	file.seekg(0, ios::end);
//	auto size = file.tellg() - begin;
//	file.seekg(pos);
//	return size;
//}
//
//int64_t GetFileBufferSizeW(wifstream& file)
//{
//	auto start = file.tellg();
//	file.seekg(0, ios::beg);
//
//	int64_t i = -1;
//	while (!file.eof() && file.get())
//		++i;
//
//	file.clear(ios_base::eofbit);
//	file.seekg(start, ios::beg);
//
//	return i;
//}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file


