#pragma once

#include "../collections/LinkedList.h"
#include "uToken.h"

class uParser
{
public:
	uParser() {}
	~uParser() {}

	void Parse(LinkedList<uAbstractToken*>* lexerTokens);
};
