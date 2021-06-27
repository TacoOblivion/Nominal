#pragma once

#include "../../collections/LinkedList.h"
#include "../lexer/uToken.hpp"
#include "../lexer/uScanner.hpp"
#include "../../basic/Types.h"
#include "uParserNodeType.hpp"

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

	void PrintDebug(int32_t depth, int32_t side);
	std::string TypeToName();
};

template <typename T>
class uParserNode : public uAbstractParserNode
{
public:
	T Data;
};

class uFuncParserNode : public uAbstractParserNode
{
public:
	uFuncParserNode();
};

class uClassParserNode : public uAbstractParserNode
{
public:
	uClassParserNode();

	bool PartialClass = false;
	std::wstring ClassName;
	std::wstring ClassParent;
	LinkedList<std::wstring> ClassInterfaces;
};

class uGlobalParserStatementListNode : public uParserNode<LinkedList<uAbstractParserNode*>*>
{
	LinkedList<uClassParserNode*> _classes;
	LinkedList<uFuncParserNode*> _funcs;

public:
	uGlobalParserStatementListNode();
	LinkedList<uClassParserNode*>& Classes() { return _classes; }
	LinkedList<uFuncParserNode*>& Funcs() { return _funcs; }
};

