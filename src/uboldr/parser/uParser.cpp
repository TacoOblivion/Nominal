
#include "uParser.h"
#include "../lexer/uScanner.h"
#include "../../collections/LinkedList.h"

void uParser::GetToken()
{
	_token = _tokens->IsEmpty() ? GetEoFToken() : _tokens->RemoveFirstItem();
}

uAbstractToken* uParser::GetEoFToken()
{
	auto token = new uAbstractToken();
	token->Type = uScannerTokenType::EoF;
	return token;
}

uAbstractToken* uParser::PeekToken()
{
	if (_tokens->IsEmpty())
		return GetEoFToken();

	auto item = _tokens->FirstItem();
	return item != nullptr ? item->Value : GetEoFToken();
}

void uParser::Parse()
{
	_tokens = _lexer.Scan();
	GetToken();

	auto statementList = StatementList();
}

void uParser::Parse(LinkedList<uAbstractToken*>* lexerTokens)
{

}

bool uParser::ExpectKeywordToken(const std::wstring ws)
{
	GetToken();

	return KeywordMatches(ws);
}

bool uParser::ExpectSymbolToken(const uSymbolType symbol)
{
	GetToken();

	return SymbolMatches(symbol);
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

LinkedList<uAbstractParserNode*>* uParser::StatementList()
{
	LinkedList<uAbstractParserNode*>* nodeList = new LinkedList<uAbstractParserNode*>();

	while (_token->Type != uScannerTokenType::EoF)
	{
		auto node = Statement();
		nodeList->AddItem(node);
	}

	return nodeList;
}

uAbstractParserNode* uParser::Statement()
{
	uAbstractParserNode* node;
	auto nextToken = PeekToken();

	if (_token->Type == uScannerTokenType::Keyword)
	{
		auto token = dynamic_cast<uToken<std::wstring>*>(_token);
		
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
		else if (token->Data == L"fn")
		{
			node = FuncDecl();
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

	auto node = new uAbstractParserNode();
	node->Type = uParserNodeType::IfStatement;

	// Expect: Symbol (
	ExpectSymbolToken(uSymbolType::ParanLeft);

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
		node->Right = BlockOrStatement();
	}

	return node;
}

uAbstractParserNode* uParser::WhileStatement()
{
	auto node = new uAbstractParserNode();
	node->Type = uParserNodeType::WhileStatement;

	// while
	GetToken();

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
	GetToken();

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
	GetToken();

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
	GetToken();

	node->Left = Expression();

	return node;
}

uAbstractParserNode* uParser::BlockOrStatement()
{
	uAbstractParserNode* node;

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

	while (_token->Type != uScannerTokenType::EoF && !SymbolMatches(uSymbolType::CurlyBraceLeft))
	{
		auto statement = Statement();
		node->Data->AddItem(node);
	}

	// Expect: Symbol }
	GetToken();

	return node;
}

uAbstractParserNode* uParser::Expression()
{
	uAbstractParserNode* node = nullptr;
	auto leftNode = Factor();

	if (SymbolMatches(uSymbolType::QuestionMark))
	{
		node = TernaryIfExpr();
		node->Left = leftNode;
	}
	else if (SymbolMatches(uSymbolType::Equals))
	{
		std::cout << "Assignment Succeeded";
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
	GetToken();

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
	GetToken();

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
			GetToken();
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
			GetToken();
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
			GetToken();
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
			GetToken();
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
			GetToken();
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
			node = new uAbstractParserNode();
			node->Type = uParserNodeType::EqualTo;
			GetToken();
		}
		else if (SymbolMatches(uSymbolType::NotEqualTo))
		{
			node = new uAbstractParserNode();
			node->Type = uParserNodeType::NotEqualTo;
			GetToken();
		}
		else if (SymbolMatches(uSymbolType::IdenticalTo))
		{
			node = new uAbstractParserNode();
			node->Type = uParserNodeType::IdenticalTo;
			GetToken();
		}
		else if (SymbolMatches(uSymbolType::NotIdenticalTo))
		{
			node = new uAbstractParserNode();
			node->Type = uParserNodeType::NotIdenticalTo;
			GetToken();
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
			GetToken();
		}
		else if (SymbolMatches(uSymbolType::LessThan))
		{
			node = new uAbstractParserNode();
			node->Type = uParserNodeType::LessThan;
			GetToken();
		}
		else if (SymbolMatches(uSymbolType::GreaterThanEqual))
		{
			node = new uAbstractParserNode();
			node->Type = uParserNodeType::GreaterThanEqual;
			GetToken();
		}
		else if (SymbolMatches(uSymbolType::LessThanEqual))
		{
			node = new uAbstractParserNode();
			node->Type = uParserNodeType::LessThanEqual;
			GetToken();
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
			GetToken();
		}
		else if (SymbolMatches(uSymbolType::BitwiseLeftShift))
		{
			node = new uAbstractParserNode();
			node->Type = uParserNodeType::BitwiseLeftShift;
			GetToken();
		}
		else if (SymbolMatches(uSymbolType::ArithmeticRightShift))
		{
			node = new uAbstractParserNode();
			node->Type = uParserNodeType::ArithmeticRightShift;
			GetToken();
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
			GetToken();
		}
		else if (SymbolMatches(uSymbolType::Minus))
		{
			node = new uAbstractParserNode();
			node->Type = uParserNodeType::Subtract;
			GetToken();
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
			GetToken();
		}
		else if (SymbolMatches(uSymbolType::Divide))
		{
			node = new uAbstractParserNode();
			node->Type = uParserNodeType::Divide;
			GetToken();
		}
		else if (SymbolMatches(uSymbolType::Percent))
		{
			node = new uAbstractParserNode();
			node->Type = uParserNodeType::Modulus;
			GetToken();
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
		GetToken();
		node->Left = ExprPreUnary();
	}
	else if (SymbolMatches(uSymbolType::BoolNot)) // should be ExclamationMark
	{
		node = new uAbstractParserNode();
		node->Type = uParserNodeType::Negate;
		GetToken();
		node->Left = ExprPreUnary();
	}
	else if (SymbolMatches(uSymbolType::Tilde))
	{
		node = new uAbstractParserNode();
		node->Type = uParserNodeType::BitwiseNot;
		GetToken();
		node->Left = ExprPreUnary();
	}
	else if (SymbolMatches(uSymbolType::Plus))
	{
		GetToken();
		node = ExprPreUnary();
	}
	else if (SymbolMatches(uSymbolType::Ampersand))
	{
		node = new uAbstractParserNode();
		node->Type = uParserNodeType::Reference;
		GetToken();
		node->Left = ExprPreUnary();
	}
	else if (SymbolMatches(uSymbolType::Increment))
	{
		node = new uAbstractParserNode();
		node->Type = uParserNodeType::PreIncrement;
		GetToken();
		node->Left = ExprPreUnary();
	}
	else if (SymbolMatches(uSymbolType::Decrement))
	{
		node = new uAbstractParserNode();
		node->Type = uParserNodeType::PreDecrement;
		GetToken();
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
			GetToken();
		}
		else if (SymbolMatches(uSymbolType::Decrement))
		{
			node = new uAbstractParserNode();
			node->Type = uParserNodeType::PostDecrement;
			GetToken();
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
			GetToken();
			node->Right = ArgumentsList();
			GetToken(); // )
		}
		else if (SymbolMatches(uSymbolType::SquareBracketLeft))
		{
			node = new uAbstractParserNode();
			node->Type = uParserNodeType::BracketAccess;
			GetToken();
			node->Right = Expression();
			GetToken(); // ]
		}
		else if (SymbolMatches(uSymbolType::Dot)) // should be Period
		{
			node = new uAbstractParserNode();
			node->Type = uParserNodeType::MemberAccess;
			GetToken();
			node->Right = Factor();
			GetToken(); // ]
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

uAbstractParserNode* uParser::Factor()
{
	uAbstractParserNode* node = nullptr;

	if (_token->Type == uScannerTokenType::IntLiteral)
	{
		auto node0 = new uParserNode<int64_t>();
		node0->Type = uParserNodeType::Integer;
		node0->Data = dynamic_cast<uToken<int64_t>*>(_token)->Data;
		GetToken();
		node = node0;
	}
	else if (_token->Type == uScannerTokenType::Identifier)
	{
		auto node0 = new uParserNode<std::wstring>();
		node0->Type = uParserNodeType::Identifier;
		node0->Data = dynamic_cast<uToken<std::wstring>*>(_token)->Data;
		GetToken();
		node = node0;
	}
	else
	{
		std::cout << "Error" << std::endl;
		GetToken();
	}

	return node;
}


