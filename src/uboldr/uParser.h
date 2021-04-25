#pragma once

#include "../collections/LinkedList.h"
#include "uToken.h"
#include "uScanner.h"
#include "../basic/Types.h"
#include "../collections/LinkedList.h"

class uParser
{
	uScanner _lexer;
	LinkedList<uAbstractToken*>* tokens = nullptr;


public:
	uParser(std::wstring path) : _lexer(path) {}
	~uParser() {}

	void Parse(LinkedList<uAbstractToken*>* lexerTokens);
	LinkedListItem<uAbstractToken*>* StatementList();

};
