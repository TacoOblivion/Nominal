#pragma once

#include "../collections/LinkedList.h"
#include "uToken.h"
#include "uScanner.h"
#include "../basic/Types.h"
#include "../collections/LinkedList.h"
#include "uParserNodeType.h"

class uAbstractParserNode
{
public:
	uAbstractParserNode* Parent = nullptr;
	uAbstractParserNode* Left = nullptr;
	uAbstractParserNode* Right = nullptr;

	uParserNodeType Type = uParserNodeType::Unknown;
	int64_t Flags = 0;

	int64_t Line = 0;
	int32_t Char = 0;
};

template <typename T>
class uParserNode : public uAbstractParserNode
{
public:
	T Data;
};

class uParser
{
	uScanner _lexer;
	LinkedList<uAbstractToken*>* _tokens = nullptr;
	uAbstractToken* _token = nullptr;
	int64_t _errorCount = 0;


public:
	uParser(std::wstring path) : _lexer(path) {}
	~uParser() {}

	void GetToken();
	uAbstractToken* PeekToken();
	void Parse();
	void Parse(LinkedList<uAbstractToken*>* lexerTokens);
	LinkedList<uAbstractParserNode*>* StatementList();
	uAbstractParserNode* Statement();
	uAbstractParserNode* Expression();
	uAbstractParserNode* Assignment();
	uAbstractParserNode* Factor();
};
