
#include "../../basic/Comparisons.h"
#include "../../basic/Conversions.h"
#include "Lexer.hpp"
#include "LexerToken.hpp"
#include "LexerTokenType.hpp"
#include "LexerSymbolType.hpp"

uScanner::uScanner(std::string path) : _file(path), _path(StrToWStr(path)), _contents(), _pos()
{
}

uScanner::uScanner(std::wstring path) : _file(path), _path(path), _contents(), _pos()
{
}

LinkedList<AbstractLexerToken*>* uScanner::Scan()
{
	// Look at the size of this damn thing
	// Alternative solution recommended for clean looks

	_pos = -1;
	auto tokens = new LinkedList<AbstractLexerToken*>();
	_file.Open(UTF8::OpenMode::Read);
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

				auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::Increment);
				tokens->AddItem(token);

				++_pos;
			}
			else if (peekOne == L'=')
			{
				// +=
				//TODO: Implement
				++_pos;
			}
			else
			{
				// +

				auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::Plus);
				tokens->AddItem(token);
			}
		}
		else if (chr == L'-')
		{
			peekOne = Peek();

			if (peekOne == L'-')
			{
				// --

				auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::Decrement);
				tokens->AddItem(token);

				++_pos;
			}
			else if (peekOne == L'=')
			{
				// -=
				//TODO: Implement
				++_pos;
			}
			else if (peekOne == L'>')
			{
				// ->

				auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::BitwiseImplication);
				tokens->AddItem(token);

				++_pos;
			}
			else
			{
				// -

				auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::Minus);
				tokens->AddItem(token);
			}
		}
		else if (chr == L'*')
		{
			// *

			auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::Asterisk);
			tokens->AddItem(token);
		}
		else if (chr == L'/')
		{
			// /

			auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::Divide);
			tokens->AddItem(token);
		}
		else if (chr == L'%')
		{
			// %

			auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::Percent);
			tokens->AddItem(token);
		}
		else if (chr == L'&')
		{
			peekOne = Peek();

			if (peekOne == L'&')
			{
				// &&

				auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::BoolAnd);
				tokens->AddItem(token);

				++_pos;
			}
			else if (peekOne == L'=')
			{
				// &=
				//TODO: Implement
				++_pos;
			}
			else
			{
				// &

				auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::Ampersand);
				tokens->AddItem(token);
			}
		}
		else if (chr == L'|')
		{
			peekOne = Peek();

			// ||
			if (peekOne == L'|')
			{
				auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::BoolOr);
				tokens->AddItem(token);

				++_pos;
			}
			// |=
			else if (peekOne == L'=')
			{
				auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::BitwiseOrEquals);
				tokens->AddItem(token);
				
				++_pos;
			}
			// |
			else
			{
				auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::Pipe);
				tokens->AddItem(token);
			}
		}
		else if (chr == L'!')
		{
			peekOne = Peek();

			if (peekOne == L'=')
			{
				peekTwo = Peek(1);

				// !==
				if (peekTwo == L'=')
				{
					auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::NotIdenticalTo);
					tokens->AddItem(token);

					_pos += 2;
				}
				// !=
				else
				{
					auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::NotEqualTo);
					tokens->AddItem(token);

					++_pos;
				}
			}
			else if (peekOne == L'>')
			{
				peekTwo = Peek(1);

				// !>=
				if (peekTwo == L'=')
				{
					auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::LessThan);
					tokens->AddItem(token);

					_pos += 2;
				}
				// !>
				else
				{
					auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::LessThanEqual);
					tokens->AddItem(token);

					++_pos;
				}
			}
			else if (peekOne == L'<')
			{
				peekTwo = Peek(1);

				// !<=
				if (peekTwo == L'=')
				{
					auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::GreaterThan);
					tokens->AddItem(token);

					_pos += 2;
				}
				// !<
				else
				{
					auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::GreaterThanEqual);
					tokens->AddItem(token);

					++_pos;
				}
			}
		}
		else if (chr == L'=')
		{
			peekOne = Peek();

			if (peekOne == L'=')
			{
				peekTwo = Peek(1);

				// ===
				if (peekTwo == L'=')
				{
					auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::IdenticalTo);
					tokens->AddItem(token);

					_pos += 2;
				}
				// ==
				else
				{
					auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::EqualTo);
					tokens->AddItem(token);

					++_pos;
				}
			}
			// =
			else
			{
				auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::Equals);
				tokens->AddItem(token);
			}
		}
		else if (chr == L'~')
		{
			peekOne = Peek();
			
			// ~=
			if (peekOne == L'=')
			{
				auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::BitwiseNotEquals);
				tokens->AddItem(token);

				++_pos;
			}
			// ~
			else
			{
				auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::Tilde);
				tokens->AddItem(token);
			}
		}
		else if (chr == L'^')
		{
			peekOne = Peek();

			// ^=
			if (peekOne == L'=')
			{
				auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::BitwiseXorEquals);
				tokens->AddItem(token);

				++_pos;
			}
			// ^
			else
			{
				auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::Circumflex);
				tokens->AddItem(token);
			}
		}
		else if (chr == L'<')
		{
			peekOne = Peek();

			if (peekOne == L'<')
			{
				peekTwo = Peek(1);

				// <<=
				if (peekTwo == L'=')
				{
					auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::BitwiseLeftShiftEquals);
					tokens->AddItem(token);

					_pos += 2;
				}
				// <<
				else
				{
					auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::BitwiseLeftShift);
					tokens->AddItem(token);

					++_pos;
				}
			}
			// <=
			else if (peekOne == L'=')
			{
				auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::LessThanEqual);
				tokens->AddItem(token);

				++_pos;
			}
			// <
			else
			{
				auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::BitwiseLeftShift);
				tokens->AddItem(token);
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
					peekThree = Peek(2);

					// >>>=
					if (peekThree == L'=')
					{
						auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::BitwiseRightShiftEquals);
						tokens->AddItem(token);

						_pos += 3;
					}
					// >>>
					else
					{
						auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::BitwiseRightShift);
						tokens->AddItem(token);

						_pos += 2;
					}
				}
				// >>=
				else if (peekTwo == L'=')
				{
					auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::ArithmeticRightShiftEquals);
					tokens->AddItem(token);

					_pos += 2;
				}
				// >>
				else
				{
					auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::ArithmeticRightShift);
					tokens->AddItem(token);

					++_pos;
				}
			}
			// >=
			else if (peekOne == L'=')
			{
				auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::GreaterThanEqual);
				tokens->AddItem(token);

				++_pos;
			}
			// >
			else
			{
				auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::GreaterThan);
				tokens->AddItem(token);
			}
		}
		else if (chr == L'?')
		{
			//TODO: Implement
			auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::QuestionMark);
			tokens->AddItem(token);
		}
		else if (chr == L':')
		{
			//TODO: Implement
			peekOne = Peek();

			if (peekOne == L'=')
			{
				auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::AutoType);
				tokens->AddItem(token);

				++_pos;
			}
			else
			{
				auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::Colon);
				tokens->AddItem(token);
			}
		}
		else if (chr == L';')
		{
			//TODO: Implement
			auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::SemiColon);
			tokens->AddItem(token);
		}
		// (
		else if (chr == L'(')
		{
			auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::ParanLeft);
			tokens->AddItem(token);
		}
		// )
		else if (chr == L')')
		{
			auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::ParanRight);
			tokens->AddItem(token);
		}
		// {
		else if (chr == L'{')
		{
			auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::CurlyBraceLeft);
			tokens->AddItem(token);
		}
		// }
		else if (chr == L'}')
		{
			auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::CurlyBraceRight);
			tokens->AddItem(token);
		}
		// [
		else if (chr == L'[')
		{
			auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::SquareBracketLeft);
			tokens->AddItem(token);
		}
		// ]
		else if (chr == L']')
		{
			auto token = CreateToken<uSymbolType>(uScannerTokenType::Symbol, uSymbolType::SquareBracketRight);
			tokens->AddItem(token);
		}
		else if (chr == L'"')
		{
			//TODO: Implement Double Quoted Item
		}
		else if (chr == L'\'')
		{
			//TODO: Implement Single Quoted Item
		}
		// Keywords & Identifiers
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
			auto tokenType = IsKeyword(ident) ? uScannerTokenType::Keyword : uScannerTokenType::Identifier;
			auto token = CreateToken<std::wstring>(tokenType, ident);
			tokens->AddItem(token);

			--_pos;
		}
		// Ints & Floats
		else if (IsNumeric(chr))
		{
			auto nextChr = Peek();

			if (IsNumeric(chr) || (chr == L'.' && IsNumeric(nextChr)))
			{
				int64_t numStart = _pos;
				int64_t numEnd = _pos + 1;
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
	int64_t newPos = _pos + pos + 1;

	if (newPos < _file.Size())
		return _contents[newPos];

	return L'\0';
}

wchar_t uScanner::GetChar()
{
	if (_pos < _file.Size())
		return _contents[_pos];

	return L'\0';
}

wchar_t uScanner::GetNextChar()
{
	if (++_pos < _file.Size())
		return _contents[_pos];

	return L'\0';
}

inline bool uScanner::IsUpperAlpha(wchar_t chr) { return chr >= L'A' && chr <= L'Z'; }

inline bool uScanner::IsLowerAlpha(wchar_t chr) { return chr >= L'a' && chr <= L'z'; }

inline bool uScanner::IsAlpha(wchar_t chr) { return IsUpperAlpha(chr) || IsLowerAlpha(chr); }

inline bool uScanner::IsNumeric(wchar_t chr) { return chr >= L'0' && chr <= L'9'; }

inline bool uScanner::IsAlphanumeric(wchar_t chr) { return IsAlpha(chr) || IsNumeric(chr); }

//TODO: Implement a much, much, much faster version of this. At the moment, I just want it to work...
inline bool uScanner::IsKeyword(std::wstring wstr)
{
	for (int32_t i = 0; i < KeywordCount; ++i)
		if (WStrEqualsStr(wstr, Keywords[i]))
			return true;

	return false;
}

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

std::string uScanner::TokenTypeToString(uScannerTokenType type)
{
	switch (type)
	{
	case uScannerTokenType::Unknown:
		return "Unknown";
	case uScannerTokenType::IntLiteral:
		return "Integer";
	case uScannerTokenType::FloatLiteral:
		return "Float";
	case uScannerTokenType::StringLiteral:
		return "String";
	case uScannerTokenType::Identifier:
		return "Identifier";
	case uScannerTokenType::Keyword:
		return "Keyword";
	case uScannerTokenType::Symbol:
		return "Symbol";
	case uScannerTokenType::EoF:
		return "EoF";
	}

	return "Error";
}

std::string uScanner::SymbolToString(uSymbolType symbol)
{
	switch (symbol)
	{
	case uSymbolType::Unknown:
		return "Unknown";
	case uSymbolType::Plus:
		return "+";
	case uSymbolType::Minus:
		return "-";
	case uSymbolType::Asterisk:
		return "*";
	case uSymbolType::Divide:
		return "/";
	case uSymbolType::Percent:
		return "%";
	case uSymbolType::Increment:
		return "++";
	case uSymbolType::Decrement:
		return "--";
	case uSymbolType::ArithmeticRightShift:
		return ">>";
	case uSymbolType::BoolAnd:
		return "&&";
	case uSymbolType::BoolOr:
		return "||";
	case uSymbolType::BoolNot:
		return "!";
	case uSymbolType::EqualTo:
		return "==";
	case uSymbolType::NotEqualTo:
		return "!=";
	case uSymbolType::IdenticalTo:
		return "===";
	case uSymbolType::NotIdenticalTo:
		return "!==";
	case uSymbolType::GreaterThan:
		return ">";
	case uSymbolType::GreaterThanEqual:
		return ">=";
	case uSymbolType::LessThan:
		return "<";
	case uSymbolType::LessThanEqual:
		return "<=";
	case uSymbolType::NullCoalescing:
		return "??";
	case uSymbolType::Ampersand:
		return "&";
	case uSymbolType::Pipe:
		return "|";
	case uSymbolType::Tilde:
		return "~";
	case uSymbolType::Circumflex:
		return "^";
	case uSymbolType::BitwiseImplication:
		return "->";
	case uSymbolType::BitwiseLeftShift:
		return "<<";
	case uSymbolType::BitwiseRightShift:
		return ">>>";
	case uSymbolType::Equals:
		return "=";
	case uSymbolType::PlusEquals:
		return "+=";
	case uSymbolType::MinusEquals:
		return "-=";
	case uSymbolType::TimesEquals:
		return "*=";
	case uSymbolType::DividedByEquals:
		return "/=";
	case uSymbolType::ModuloEquals:
		return "%=";
	case uSymbolType::ArithmeticRightShiftEquals:
		return ">>=";
	case uSymbolType::BitwiseAndEquals:
		return "&=";
	case uSymbolType::BitwiseOrEquals:
		return "|=";
	case uSymbolType::BitwiseNotEquals:
		return "~=";
	case uSymbolType::BitwiseXorEquals:
		return "^=";
	case uSymbolType::BitwiseImplicationEquals:
		return "->=";
	case uSymbolType::BitwiseLeftShiftEquals:
		return "<<=";
	case uSymbolType::BitwiseRightShiftEquals:
		return ">>>=";
	case uSymbolType::NullCoalescingEquals:
		return "??=";
	case uSymbolType::ParanLeft:
		return "(";
	case uSymbolType::ParanRight:
		return ")";
	case uSymbolType::SquareBracketLeft:
		return "[";
	case uSymbolType::SquareBracketRight:
		return "]";
	case uSymbolType::CurlyBraceLeft:
		return "{";
	case uSymbolType::CurlyBraceRight:
		return "}";
	case uSymbolType::NullCoalescingSquareBracket:
		return "?[";
	case uSymbolType::Dot:
		return ".";
	case uSymbolType::NullCoalescingAccess:
		return "?.";
	case uSymbolType::Range:
		return "..";
	case uSymbolType::Sharp:
		return "#";
	case uSymbolType::At:
		return "@";
	case uSymbolType::QuestionMark:
		return "?";
	case uSymbolType::Colon:
		return ":";
	case uSymbolType::SemiColon:
		return ";";
	}

	return "Error";
}
