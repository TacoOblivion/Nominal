#pragma once

/*#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <codecvt>
#include <cstdlib>
#include <ios>
#include <iosfwd>*/
//#include <bits/ios_base.h>
#include <xiosbase>

namespace UTF8
{

	// of mode|out|trunc
	// if mode|in
	//  f mode
	enum class OpenMode : int32_t
	{
		/// Open for input.
		Read = 1, // std::ios_base::in,
		/// Open for output.
		Write = 2, // std::ios_base::out,
		/// Open for input.
		Truncate = 16, // std::ios_base::trunc,
		/// Seek to end before each write.
		Append = 8, // std::ios_base::app,
		/// Open and seek to end immediately after opening.
		AppendToEnd = 4, // std::ios_base::ate,
		/// Perform input and output in binary mode (as opposed to text mode).
		/// This is probably not what you think it is; see
		/// https://gcc.gnu.org/onlinedocs/libstdc++/manual/fstreams.html#std.io.filestreams.binary
		Binary = 32, //std::ios_base::binary

		DefaultRead = Read,
		DefaultWrite = Write | Truncate,
		DefaultReadWrite = Read | Write,
	};

}
