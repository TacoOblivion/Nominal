
#include "Parser.hpp"
#include "../lexer/Lexer.hpp"
#include "../../collections/LinkedList.h"

void uParser::GetNextToken()
{
	if (!_tokens->IsEmpty())
		std::cout << "Token Retrieved: " << (int32_t)_tokens->FirstItem()->Value->Type << std::endl;
	_token = _tokens->IsEmpty() ? GetEoFToken() : _tokens->RemoveFirstItem();
}

AbstractLexerToken* uParser::GetEoFToken()
{
	auto token = new AbstractLexerToken();
	token->Type = uScannerTokenType::EoF;
	return token;
}

AbstractLexerToken* uParser::PeekToken()
{
	if (_tokens->IsEmpty())
		return GetEoFToken();

	auto item = _tokens->FirstItem();
	return item != nullptr ? item->Value : GetEoFToken();
}

uAbstractParserNode* uParser::Parse()
{
	_tokens = _lexer.Scan();
	GetNextToken();

	auto statementList = GlobalStatementList();
	return statementList;
}

uAbstractParserNode* uParser::Parse(LinkedList<AbstractLexerToken*>* lexerTokens)
{
	return nullptr;
}

bool uParser::ExpectKeywordToken(const std::wstring ws)
{
	auto r = KeywordMatches(ws);

	if (!r)
	{
		std::cout << std::endl << "Expected Keyword \"";

		for (int32_t i = 0; i < ws.length(); ++i)
			std::cout << (char)ws[i];

		std::cout << "\"" << std::endl;
	}

	GetNextToken();

	return r;
}

bool uParser::ExpectSymbolToken(const uSymbolType symbol)
{
	auto r = SymbolMatches(symbol);

	if (!r)
		std::cout << "Expected Symbol " << (int32_t)(int8_t)symbol << std::endl;

	GetNextToken();

	return r;
}

bool uParser::KeywordMatches(const std::wstring ws)
{
	return _token->Type == uScannerTokenType::Keyword && _token->GetData<std::wstring>() == ws;
}

bool uParser::SymbolMatches(const uSymbolType symbol)
{
	return _token->Type == uScannerTokenType::Symbol && _token->GetData<uSymbolType>() == symbol;
}

bool uParser::InSymbolRange(const uSymbolType low, const uSymbolType high)
{
	if (_token->Type != uScannerTokenType::Symbol)
		return false;

	auto symbol = _token->GetData<uSymbolType>();
	return symbol >= low && symbol <= high;
}

uAbstractParserNode* uParser::GlobalStatementList()
{
	auto node = new uGlobalParserStatementListNode();
	auto nodeList = new LinkedList<uAbstractParserNode*>();

	while (_token->Type != uScannerTokenType::EoF)
	{
		auto stmtNode = GlobalStatement();

		if (stmtNode->Type == uParserNodeType::FnDef)
		{
			node->Funcs().AddItem((uFuncParserNode*)stmtNode);
		}
		else if (stmtNode->Type == uParserNodeType::ClassDef)
		{
			node->Classes().AddItem((uClassParserNode*)stmtNode);
		}
		else
		{
			nodeList->AddItem(stmtNode);
		}
	}

	node->Data = nodeList;
	return node;
}

uAbstractParserNode* uParser::GlobalStatement()
{
	uAbstractParserNode* node = nullptr;

	if (KeywordMatches(L"fn"))
	{
		node = FuncDef();
	}
	else if (KeywordMatches(L"class"))
	{
		node = ClassDef();
	}
	else if (KeywordMatches(L"partial"))
	{
		node = ClassDef(true);
	}
	else
	{
		node = Statement();
	}

	return node;
}


uFuncParserNode* uParser::FuncDef()
{
	uFuncParserNode* node = new uFuncParserNode();

	ExpectKeywordToken(L"fn");



	return node;
}


uClassParserNode* uParser::ClassDef(bool partial)
{
	uClassParserNode* node = new uClassParserNode();


	if (partial)
	{
		ExpectKeywordToken(L"partial");
		node->PartialClass = true;
	}

	ExpectKeywordToken(L"class");

	return node;
}

uAbstractParserNode* uParser::Statement()
{
	uAbstractParserNode* node = nullptr;
	//auto nextToken = PeekToken();

	if (_token->Type == uScannerTokenType::Keyword)
	{
		auto token = dynamic_cast<LexerToken<std::wstring>*>(_token);
		
		if (token->Data == L"if")
		{
			node = IfStatement();
		}
		else if (token->Data == L"while")
		{
			node = WhileStatement();
		}
		else if (token->Data == L"for")
		{
			node = ForStatement();
		}
		else if (token->Data == L"return")
		{
			//node = ReturnStatement();
		}
		else
		{
			node = Expression();
		}
	}
	else
	{
		node = Expression();
	}

	return node;
}

uAbstractParserNode* uParser::IfStatement()
{
	// Expect: Keyword "if"
	ExpectKeywordToken(L"if");
	// Expect: Symbol (
	ExpectSymbolToken(uSymbolType::ParanLeft);

	auto node = new uAbstractParserNode();
	node->Type = uParserNodeType::IfStatement;

	node->Left = Expression();

	// Expect: Symbol )
	ExpectSymbolToken(uSymbolType::ParanRight);

	node->Right = IfBlock();

	return node;
}

uAbstractParserNode* uParser::IfBlock()
{
	auto node = new uAbstractParserNode();
	node->Type = uParserNodeType::IfBlock;

	node->Left = BlockOrStatement();

	if (KeywordMatches(L"else"))
	{
		GetNextToken();

		node->Right = BlockOrStatement();
	}

	return node;
}

uAbstractParserNode* uParser::WhileStatement()
{
	auto node = new uAbstractParserNode();
	node->Type = uParserNodeType::WhileStatement;

	// while
	ExpectKeywordToken(L"while");

	// check token == (
	ExpectSymbolToken(uSymbolType::ParanLeft);

	node->Left = Expression();

	// check token == )
	ExpectSymbolToken(uSymbolType::ParanRight);

	node->Right = BlockOrStatement();

	return node;
}

uAbstractParserNode* uParser::ForStatement()
{
	auto node = new uAbstractParserNode();
	node->Type = uParserNodeType::ForStatement;

	// for
	GetNextToken();

	// check token == (
	ExpectSymbolToken(uSymbolType::ParanLeft);

	node->Left = ForStatementListInitializer();

	// check token == )
	ExpectSymbolToken(uSymbolType::ParanRight);

	node->Right = BlockOrStatement();

	return node;
}

uAbstractParserNode* uParser::ForStatementListInitializer()
{
	auto node = new uAbstractParserNode();
	node->Type = uParserNodeType::ForStatementListInitializer;

	node->Left = Expression();

	if (_token->GetData<uSymbolType>() == uSymbolType::SemiColon)
	{
		node->Right = ForStatementListCondition();
	}

	return node;
}

uAbstractParserNode* uParser::ForStatementListCondition()
{
	auto node = new uAbstractParserNode();
	node->Type = uParserNodeType::ForStatementListCondition;

	// ;
	GetNextToken();

	node->Left = Expression();

	if (_token->GetData<uSymbolType>() == uSymbolType::SemiColon)
	{
		node->Right = ForStatementListCondition();
	}

	return node;
}

uAbstractParserNode* uParser::ForStatementListLoopCounter()
{
	auto node = new uAbstractParserNode();
	node->Type = uParserNodeType::ForStatementListLoopCounter;

	// ;
	GetNextToken();

	node->Left = Expression();

	return node;
}

uAbstractParserNode* uParser::BlockOrStatement()
{
	uAbstractParserNode* node = nullptr;

	if (SymbolMatches(uSymbolType::CurlyBraceLeft))
	{
		node = Block();
	}
	else
	{
		node = Statement();
	}

	return node;
}

uAbstractParserNode* uParser::Block()
{
	// Expect: Symbol {
	ExpectSymbolToken(uSymbolType::CurlyBraceLeft);

	auto node = new uParserNode<LinkedList<uAbstractParserNode*>*>();
	node->Type = uParserNodeType::StatementList;
	node->Data = new LinkedList<uAbstractParserNode*>();

	while (_token->Type != uScannerTokenType::EoF && !SymbolMatches(uSymbolType::CurlyBraceRight))
	{
		auto statement = Statement();
		node->Data->AddItem(statement);
	}

	// Expect: Symbol }
	ExpectSymbolToken(uSymbolType::CurlyBraceRight);

	return node;
}

uAbstractParserNode* uParser::Expression()
{
	uAbstractParserNode* node = nullptr;
	auto leftNode = ExprBoolOr();

	if (SymbolMatches(uSymbolType::QuestionMark))
	{
		node = TernaryIfExpr();
		node->Left = leftNode;
	}
	else if (SymbolMatches(uSymbolType::Equals))
	{
		std::cout << "Assignment" << std::endl;
		node = Assignment();
		node->Left = leftNode;
	}
	else if (InSymbolRange(uSymbolType::PlusEquals, uSymbolType::NullCoalescingEquals))
	{
		node = AugmentedAssignment();
		node->Left = leftNode;
		node->Right->Left = leftNode;
	}

	if (node == nullptr)
		node = leftNode;

	return node;
}

uAbstractParserNode* uParser::TernaryIfExpr()
{
	uAbstractParserNode* node = new uAbstractParserNode();
	node->Type = uParserNodeType::IfStatement;

	// ?
	GetNextToken();

	node->Right = TernaryIfBlock();

	return node;
}

uAbstractParserNode* uParser::TernaryIfBlock()
{
	uAbstractParserNode* node = new uAbstractParserNode();
	node->Type = uParserNodeType::TernaryIfBlock;

	node->Left = Expression();

	ExpectSymbolToken(uSymbolType::Colon);

	node->Right = Expression();

	return node;
}

uAbstractParserNode* uParser::Assignment()
{
	uAbstractParserNode* node = new uAbstractParserNode();
	node->Type = uParserNodeType::Assignment;

	// ensure than symbol data == uSymbolType::Equals
	GetNextToken();

	node->Right = Expression();

	return node;
}

uAbstractParserNode* uParser::AugmentedAssignment()
{
	uAbstractParserNode* node = new uAbstractParserNode();
	node->Type = uParserNodeType::Assignment;

	if (SymbolMatches(uSymbolType::PlusEquals))
	{
		node->Right = new uAbstractParserNode();
		node->Right->Type = uParserNodeType::Add;
		node->Right->Right = Expression();
	}

	return node;
}

uAbstractParserNode* uParser::ExprBoolOr()
{
	uAbstractParserNode* leftNode = ExprBoolAnd();
	uAbstractParserNode* node = nullptr;

	while (_token->Type != uScannerTokenType::EoF)
	{
		if (node != nullptr)
			leftNode = node;

		if (SymbolMatches(uSymbolType::BoolOr))
		{
			node = new uAbstractParserNode();
			node->Type = uParserNodeType::BoolOr;
			GetNextToken();
		}
		else
		{
			break;
		}

		node->Left = leftNode;
		node->Right = ExprBoolAnd();
	}

	if (node == nullptr)
		node = leftNode;

	return node;
}

uAbstractParserNode* uParser::ExprBoolAnd()
{
	uAbstractParserNode* leftNode = ExprBitwiseOr();
	uAbstractParserNode* node = nullptr;

	while (_token->Type != uScannerTokenType::EoF)
	{
		if (node != nullptr)
			leftNode = node;

		if (SymbolMatches(uSymbolType::BoolAnd))
		{
			node = new uAbstractParserNode();
			node->Type = uParserNodeType::BoolAnd;
			GetNextToken();
		}
		else
		{
			break;
		}

		node->Left = leftNode;
		node->Right = ExprBitwiseOr();
	}

	if (node == nullptr)
		node = leftNode;

	return node;
}

uAbstractParserNode* uParser::ExprBitwiseOr()
{
	uAbstractParserNode* leftNode = ExprBitwiseXor();
	uAbstractParserNode* node = nullptr;

	while (_token->Type != uScannerTokenType::EoF)
	{
		if (node != nullptr)
			leftNode = node;

		if (SymbolMatches(uSymbolType::Pipe))
		{
			node = new uAbstractParserNode();
			node->Type = uParserNodeType::BitwiseOr;
			GetNextToken();
		}
		else
		{
			break;
		}

		node->Left = leftNode;
		node->Right = ExprBitwiseXor();
	}

	if (node == nullptr)
		node = leftNode;

	return node;
}

uAbstractParserNode* uParser::ExprBitwiseXor()
{
	uAbstractParserNode* leftNode = ExprBitwiseAnd();
	uAbstractParserNode* node = nullptr;

	while (_token->Type != uScannerTokenType::EoF)
	{
		if (node != nullptr)
			leftNode = node;

		if (SymbolMatches(uSymbolType::Circumflex))
		{
			node = new uAbstractParserNode();
			node->Type = uParserNodeType::BitwiseXor;
			GetNextToken();
		}
		else
		{
			break;
		}

		node->Left = leftNode;
		node->Right = ExprBitwiseAnd();
	}

	if (node == nullptr)
		node = leftNode;

	return node;
}

uAbstractParserNode* uParser::ExprBitwiseAnd()
{
	uAbstractParserNode* leftNode = ExprEquality();
	uAbstractParserNode* node = nullptr;

	while (_token->Type != uScannerTokenType::EoF)
	{
		if (node != nullptr)
			leftNode = node;

		if (SymbolMatches(uSymbolType::Ampersand))
		{
			node = new uAbstractParserNode();
			node->Type = uParserNodeType::BitwiseAnd;
			GetNextToken();
		}
		else
		{
			break;
		}

		node->Left = leftNode;
		node->Right = ExprEquality();
	}

	if (node == nullptr)
		node = leftNode;

	return node;
}

uAbstractParserNode* uParser::ExprEquality()
{
	uAbstractParserNode* leftNode = ExprRelational();
	uAbstractParserNode* node = nullptr;

	while (_token->Type != uScannerTokenType::EoF)
	{
		if (node != nullptr)
			leftNode = node;

		if (SymbolMatches(uSymbolType::EqualTo))
		{
			std::cout << "EqualTo" << std::endl;
			node = new uAbstractParserNode();
			node->Type = uParserNodeType::EqualTo;
			GetNextToken();
		}
		else if (SymbolMatches(uSymbolType::NotEqualTo))
		{
			node = new uAbstractParserNode();
			node->Type = uParserNodeType::NotEqualTo;
			GetNextToken();
		}
		else if (SymbolMatches(uSymbolType::IdenticalTo))
		{
			node = new uAbstractParserNode();
			node->Type = uParserNodeType::IdenticalTo;
			GetNextToken();
		}
		else if (SymbolMatches(uSymbolType::NotIdenticalTo))
		{
			node = new uAbstractParserNode();
			node->Type = uParserNodeType::NotIdenticalTo;
			GetNextToken();
		}
		else
		{
			break;
		}

		node->Left = leftNode;
		node->Right = ExprRelational();
	}

	if (node == nullptr)
		node = leftNode;

	return node;
}

uAbstractParserNode* uParser::ExprRelational()
{
	uAbstractParserNode* leftNode = ExprBitShift();
	uAbstractParserNode* node = nullptr;

	while (_token->Type != uScannerTokenType::EoF)
	{
		if (node != nullptr)
			leftNode = node;

		if (SymbolMatches(uSymbolType::GreaterThan))
		{
			node = new uAbstractParserNode();
			node->Type = uParserNodeType::GreaterThan;
			GetNextToken();
		}
		else if (SymbolMatches(uSymbolType::LessThan))
		{
			node = new uAbstractParserNode();
			node->Type = uParserNodeType::LessThan;
			GetNextToken();
		}
		else if (SymbolMatches(uSymbolType::GreaterThanEqual))
		{
			node = new uAbstractParserNode();
			node->Type = uParserNodeType::GreaterThanEqual;
			GetNextToken();
		}
		else if (SymbolMatches(uSymbolType::LessThanEqual))
		{
			node = new uAbstractParserNode();
			node->Type = uParserNodeType::LessThanEqual;
			GetNextToken();
		}
		else
		{
			break;
		}

		node->Left = leftNode;
		node->Right = ExprBitShift();
	}

	if (node == nullptr)
		node = leftNode;

	return node;
}

uAbstractParserNode* uParser::ExprBitShift()
{
	uAbstractParserNode* leftNode = ExprMathAddSub();
	uAbstractParserNode* node = nullptr;

	while (_token->Type != uScannerTokenType::EoF)
	{
		if (node != nullptr)
			leftNode = node;

		if (SymbolMatches(uSymbolType::BitwiseRightShift))
		{
			node = new uAbstractParserNode();
			node->Type = uParserNodeType::BitwiseRightShift;
			GetNextToken();
		}
		else if (SymbolMatches(uSymbolType::BitwiseLeftShift))
		{
			node = new uAbstractParserNode();
			node->Type = uParserNodeType::BitwiseLeftShift;
			GetNextToken();
		}
		else if (SymbolMatches(uSymbolType::ArithmeticRightShift))
		{
			node = new uAbstractParserNode();
			node->Type = uParserNodeType::ArithmeticRightShift;
			GetNextToken();
		}
		else
		{
			break;
		}

		node->Left = leftNode;
		node->Right = ExprMathAddSub();
	}

	if (node == nullptr)
		node = leftNode;

	return node;
}

uAbstractParserNode* uParser::ExprMathAddSub()
{
	uAbstractParserNode* leftNode = ExprMathMulDivMod();
	uAbstractParserNode* node = nullptr;

	while (_token->Type != uScannerTokenType::EoF)
	{
		if (node != nullptr)
			leftNode = node;

		if (SymbolMatches(uSymbolType::Plus))
		{
			node = new uAbstractParserNode();
			node->Type = uParserNodeType::Add;
			GetNextToken();
		}
		else if (SymbolMatches(uSymbolType::Minus))
		{
			node = new uAbstractParserNode();
			node->Type = uParserNodeType::Subtract;
			GetNextToken();
		}
		else
		{
			break;
		}

		node->Left = leftNode;
		node->Right = ExprMathMulDivMod();
	}

	if (node == nullptr)
		node = leftNode;

	return node;
}

uAbstractParserNode* uParser::ExprMathMulDivMod()
{
	uAbstractParserNode* leftNode = ExprPreUnary();
	uAbstractParserNode* node = nullptr;

	while (_token->Type != uScannerTokenType::EoF)
	{
		if (node != nullptr)
			leftNode = node;

		if (SymbolMatches(uSymbolType::Asterisk))
		{
			node = new uAbstractParserNode();
			node->Type = uParserNodeType::Multiply;
			GetNextToken();
		}
		else if (SymbolMatches(uSymbolType::Divide))
		{
			node = new uAbstractParserNode();
			node->Type = uParserNodeType::Divide;
			GetNextToken();
		}
		else if (SymbolMatches(uSymbolType::Percent))
		{
			node = new uAbstractParserNode();
			node->Type = uParserNodeType::Modulus;
			GetNextToken();
		}
		else
		{
			break;
		}

		node->Left = leftNode;
		node->Right = ExprPreUnary();
	}

	if (node == nullptr)
		node = leftNode;

	return node;
}

uAbstractParserNode* uParser::ExprPreUnary()
{
	uAbstractParserNode* node = nullptr;

	if (SymbolMatches(uSymbolType::Minus))
	{
		node = new uAbstractParserNode();
		node->Type = uParserNodeType::Negate;
		GetNextToken();
		node->Left = ExprPreUnary();
	}
	else if (SymbolMatches(uSymbolType::BoolNot)) // should be ExclamationMark
	{
		node = new uAbstractParserNode();
		node->Type = uParserNodeType::Negate;
		GetNextToken();
		node->Left = ExprPreUnary();
	}
	else if (SymbolMatches(uSymbolType::Tilde))
	{
		node = new uAbstractParserNode();
		node->Type = uParserNodeType::BitwiseNot;
		GetNextToken();
		node->Left = ExprPreUnary();
	}
	else if (SymbolMatches(uSymbolType::Plus))
	{
		GetNextToken();
		node = ExprPreUnary();
	}
	else if (SymbolMatches(uSymbolType::Ampersand))
	{
		node = new uAbstractParserNode();
		node->Type = uParserNodeType::Reference;
		GetNextToken();
		node->Left = ExprPreUnary();
	}
	else if (SymbolMatches(uSymbolType::Increment))
	{
		node = new uAbstractParserNode();
		node->Type = uParserNodeType::PreIncrement;
		GetNextToken();
		node->Left = ExprPreUnary();
	}
	else if (SymbolMatches(uSymbolType::Decrement))
	{
		node = new uAbstractParserNode();
		node->Type = uParserNodeType::PreDecrement;
		GetNextToken();
		node->Left = ExprPreUnary();
	}
	else
	{
		node = ExprPostUnary();
	}

	return node;
}

uAbstractParserNode* uParser::ExprPostUnary()
{
	uAbstractParserNode* node = nullptr;
	uAbstractParserNode* leftNode = ExprCallAndMemberAccess();

	while (_token->Type != uScannerTokenType::EoF)
	{
		if (node != nullptr)
			leftNode = node;

		if (SymbolMatches(uSymbolType::Increment))
		{
			node = new uAbstractParserNode();
			node->Type = uParserNodeType::PostIncrement;
			GetNextToken();
		}
		else if (SymbolMatches(uSymbolType::Decrement))
		{
			node = new uAbstractParserNode();
			node->Type = uParserNodeType::PostDecrement;
			GetNextToken();
		}
		else
		{
			break;
		}

		node->Left = leftNode;
	}

	if (node == nullptr)
		node = leftNode;

	return node;
}

uAbstractParserNode* uParser::ExprCallAndMemberAccess()
{
	uAbstractParserNode* leftNode = Factor();
	uAbstractParserNode* node = nullptr;

	while (_token->Type != uScannerTokenType::EoF)
	{
		if (node != nullptr)
			leftNode = node;

		if (SymbolMatches(uSymbolType::ParanLeft))
		{
			node = new uAbstractParserNode();
			node->Type = uParserNodeType::FnCall;
			GetNextToken();
			node->Right = ArgumentsList();
			GetNextToken(); // )
		}
		else if (SymbolMatches(uSymbolType::SquareBracketLeft))
		{
			node = new uAbstractParserNode();
			node->Type = uParserNodeType::BracketAccess;
			GetNextToken();
			node->Right = Expression();
			GetNextToken(); // ]
		}
		else if (SymbolMatches(uSymbolType::Dot)) // should be Period
		{
			node = new uAbstractParserNode();
			node->Type = uParserNodeType::MemberAccess;
			GetNextToken();
			node->Right = Factor();
			GetNextToken(); // ]
		}
		else
		{
			break;
		}

		node->Left = leftNode;
		node->Right = ExprMathMulDivMod();
	}

	if (node == nullptr)
		node = leftNode;

	return node;
}

uAbstractParserNode* uParser::ArgumentsList()
{
	return nullptr;
}

uAbstractParserNode* uParser::Factor()
{
	uAbstractParserNode* node = nullptr;

	if (_token->Type == uScannerTokenType::IntLiteral)
	{
		std::cout << "Integer" << std::endl;
		auto node0 = new uParserNode<int64_t>();
		node0->Type = uParserNodeType::Integer;
		node0->Data = dynamic_cast<LexerToken<int64_t>*>(_token)->Data;
		GetNextToken();
		node = node0;
	}
	else if (_token->Type == uScannerTokenType::Identifier)
	{
		std::cout << "Identifier" << std::endl;
		auto node0 = new uParserNode<std::wstring>();
		node0->Type = uParserNodeType::Identifier;
		node0->Data = dynamic_cast<LexerToken<std::wstring>*>(_token)->Data;
		GetNextToken();
		node = node0;
	}
	else
	{
		std::cout << "{Error}" << std::endl;
		GetNextToken();
	}

	return node;
}


