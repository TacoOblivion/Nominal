
#include "uScanner.h"
#include "uToken.h"
#include "uScannerTokenType.h"
#include "uTokenType.h"

inline uScanner::uScanner(std::string path) : _file(path), _path(path), _contents(), _pos()
{
}

LinkedList<object_t*>* uScanner::Scan()
{
	_pos = -1;
	auto tokens = new LinkedList<object_t*>();
	_file.Open(UTF8OpenMode::Read);
	_contents = _file.ReadAll();
	_file.Close();

	wchar_t peekOne;
	wchar_t peekTwo;
	wchar_t peekThree;
	wchar_t peekFour;
	wchar_t chr;

	while (GetNextChar() != L'\0')
	{
		chr = GetChar();

		if (chr == L'+')
		{
			peekOne = Peek();

			if (peekOne == L'+')
			{
				// ++
			}
			else if (peekOne == L'=')
			{
				// +=
			}
			else
			{
				// +
			}
		}
		else if (chr == L'-')
		{
			peekOne = Peek();

			if (peekOne == L'-')
			{
				// --
			}
			else if (peekOne == L'=')
			{
				// -=
			}
			else if (peekOne == L'>')
			{
				// ->
			}
			else
			{
				// -
			}
		}
		else if (chr == L'*')
		{
			// *
		}
		else if (chr == L'/')
		{
			// /
		}
		else if (chr == L'%')
		{
			// %
		}
		else if (chr == L'&')
		{
			peekOne = Peek();

			if (peekOne == L'&')
			{
				// &&
			}
			else if (peekOne == L'=')
			{
				// &=
			}
			else
			{
				// &
			}
		}
		else if (chr == L'|')
		{
			peekOne = Peek();

			if (peekOne == L'|')
			{
				// ||
			}
			else if (peekOne == L'=')
			{
				// |=
			}
			else
			{
				// |
			}
		}
		else if (chr == L'!')
		{
			peekOne = Peek();

			if (peekOne == L'=')
			{
				peekTwo = Peek(1);

				if (peekTwo == L'=')
				{
					// !==
				}
				else
				{
					// !=
				}
			}
			else if (peekOne == L'>')
			{
				peekTwo = Peek(1);

				if (peekTwo == L'=')
				{
					// !>=
				}
				else
				{
					// !>
				}
			}
			else if (peekOne == L'<')
			{
				peekTwo = Peek(1);

				if (peekTwo == L'=')
				{
					// !<=
				}
				else
				{
					// !<
				}
			}
		}
		else if (chr == L'=')
		{
			peekOne = Peek();

			if (peekOne == L'=')
			{
				peekTwo = Peek(1);

				if (peekTwo == L'=')
				{
					// ===
				}
				else
				{
					// ==
				}
			}
			else
			{
				// =

				auto token = CreateToken<uParserTokenType>(uScannerTokenType::Symbol, uParserTokenType::Assignment);
				tokens->AddItem(token);
			}
		}
		else if (chr == L'~')
		{
			// ~
		}
		else if (chr == L'^')
		{
			// ^
		}
		else if (chr == L'<')
		{
			peekOne = Peek();

			if (peekOne == L'<')
			{
				// <<
			}
			else if (peekOne == L'=')
			{
				// <=
			}
			else
			{
				// <
			}
		}
		else if (chr == L'>')
		{
			peekOne = Peek();

			if (peekOne == L'>')
			{
				peekTwo = Peek(1);

				if (peekTwo == L'>')
				{
					// >>>
				}
				else
				{
					// >>
				}
			}
			else if (peekOne == L'=')
			{
				// >=
			}
			else
			{
				// >
			}
		}
		else if (chr == L'(')
		{
			// (
		}
		else if (chr == L')')
		{
			// )
		}
		else if (chr == L'{')
		{
			// {
		}
		else if (chr == L'}')
		{
			// }
		}
		else if (chr == L'[')
		{
			// [
		}
		else if (chr == L']')
		{
			// ]
		}
		else if (chr == L'_' || IsAlpha(chr))
		{
			int64_t identStart = _pos;
			int64_t identEnd = _pos + 1;

			chr = GetNextChar();
			while (IsAlphanumeric(chr) || chr == L'_' || chr > 128)
			{
				if (chr == -1)
					//TODO: Unexpected identifier termination
					//throw new ScannerException();
					;

				++identEnd;
				chr = GetNextChar();
			}

			std::wstring ident = ReadWString(identStart, identEnd - identStart);
			auto token = CreateToken<std::wstring>(uScannerTokenType::StringLiteral, ident);
			tokens->AddItem(token);

			/*Keyword keyword = GetKeyword(ident);
			if (keyword != null)
				scanToken = CreateToken(eToken.Keyword, keyword.Name);
			else
				scanToken = CreateToken(eToken.Identifier, ident);

			--scanDex;
			*/
			--_pos;
		}
		else if (IsNumeric(chr))
		{
			auto nextChr = Peek();

			if (IsNumeric(chr) || (chr == L'.' && IsNumeric(nextChr)))
			{
				int64_t numStart = chr;
				int64_t numEnd = chr + 1;
				bool foundDot = false, foundExp = false, foundSign = false;

				while (nextChr != L'\0')
				{
					if (foundExp && !foundSign)
					{
						if (nextChr == L'+' || nextChr == L'-')
						{
							++numEnd;
							foundSign = true;
						}
						else
						{
							//TODO: Malformed float
							//throw new ScannerException();
						}
					}
					else if (IsNumeric(nextChr))
					{
						++numEnd;
					}
					else if (nextChr == L'.')
					{
						if (foundDot || foundExp || foundSign)
						{
							//TODO: Malformed float
							//throw new ScannerException();
						}
						else
						{
							++numEnd;
							foundDot = true;
						}
					}
					else if (nextChr == L'E' || nextChr == L'e')
					{
						if (foundExp || foundSign)
						{
							//TODO: Malformed float
							//throw new ScannerException();
						}
						else
						{
							++numEnd;
							foundExp = true;
						}
					}
					else
					{
						break;
					}

					++_pos;
					nextChr = Peek();
				}

				if (foundDot)
				{
					auto floatStr = ReadString(numStart, numEnd - numStart);
					float64_t floatValue = std::stold(floatStr);
					auto token = CreateToken<float64_t>(uScannerTokenType::FloatLiteral, floatValue);
					tokens->AddItem(token);
				}
				else
				{
					auto intStr = ReadString(numStart, numEnd - numStart);
					int64_t intValue = std::stoll(intStr);
					auto token = CreateToken<int64_t>(uScannerTokenType::IntLiteral, intValue);
					tokens->AddItem(token);
				}
			}
		}
	}

	return tokens;
}

wchar_t uScanner::Peek(int64_t pos)
{
	int32_t newPos = _pos + pos + 1;

	if (newPos < _file.BufferSize())
		return _contents[newPos];

	return L'\0';
}

wchar_t uScanner::GetChar()
{
	if (_pos < _file.BufferSize())
		return _contents[_pos];

	return L'\0';
}

wchar_t uScanner::GetNextChar()
{
	if (++_pos < _file.BufferSize())
		return _contents[_pos];

	return L'\0';
}

inline bool uScanner::IsUpperAlpha(wchar_t chr) { return chr >= L'A' && chr <= L'Z'; }

inline bool uScanner::IsLowerAlpha(wchar_t chr) { return chr >= L'a' && chr <= L'z'; }

inline bool uScanner::IsAlpha(wchar_t chr) { return IsUpperAlpha(chr) || IsLowerAlpha(chr); }

inline bool uScanner::IsNumeric(wchar_t chr) { return chr >= L'0' && chr <= L'9'; }

inline bool uScanner::IsAlphanumeric(wchar_t chr) { return IsAlpha(chr) || IsNumeric(chr); }

std::string uScanner::ReadString(int64_t startPos, int64_t length)
{
	std::string outStr;
	outStr.reserve(length);

	for (int64_t i = 0; i < length; ++i)
		outStr.push_back(_contents[startPos + i]);

	return outStr;
}

std::wstring uScanner::ReadWString(int64_t startPos, int64_t length)
{
	std::wstring outStr;
	outStr.reserve(length);
	
	for (int64_t i = 0; i < length; ++i)
		outStr.push_back(_contents[startPos + i]);

	return outStr;
}
