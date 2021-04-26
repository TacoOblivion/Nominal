
#include "uParser.h"
#include "uScanner.h"


void uParser::GetToken()
{
	if (_tokens->IsEmpty())
	{
		_token = new uAbstractToken();
		_token->Type = uScannerTokenType::EoF;
	}
	else
	{
		_token = _tokens->RemoveFirstItem();
	}
}

uAbstractToken* uParser::PeekToken()
{
	if (_tokens->IsEmpty())
	{
		// return EoF
	}

	auto item = _tokens->FirstItem();
	return item != nullptr ? item->Value : nullptr;
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
	auto node = new uAbstractParserNode();
	node->Type = uParserNodeType::Statement;
	auto nextToken = PeekToken();

	if (false)
	{

	}
	else
	{
		node->Left = Expression();
	}

	return node;
}

uAbstractParserNode* uParser::Expression()
{
	uAbstractParserNode* node = nullptr;
	auto leftNode = Factor();

	if (_token->Type == uScannerTokenType::Symbol &&
		dynamic_cast<uToken<uSymbolType>*>(_token)->Data == uSymbolType::Equals)
	{
		node = Assignment();
		node->Left = leftNode;
	}

	if (node == nullptr)
		node = leftNode;

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
