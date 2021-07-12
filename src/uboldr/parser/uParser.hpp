#pragma once

#include "../../collections/LinkedList.h"
#include "../lexer/uToken.hpp"
#include "../lexer/uScanner.hpp"
#include "../../basic/Types.h"
#include "uParserNodeType.hpp"
#include "uParserNode.hpp"

class uParser
{
	uScanner _lexer;
	LinkedList<uAbstractToken*>* _tokens = nullptr;
	uAbstractToken* _token = nullptr;
	int64_t _errorCount = 0;


public:
	uParser(std::wstring path) : _lexer(path) {}
	~uParser() {}

	void GetNextToken();
	uAbstractToken* GetEoFToken();
	uAbstractToken* PeekToken();
	uAbstractParserNode* Parse();
	uAbstractParserNode* Parse(LinkedList<uAbstractToken*>* lexerTokens);

	bool ExpectKeywordToken(const std::wstring ws);
	bool ExpectSymbolToken(const uSymbolType symbol);
	bool KeywordMatches(const std::wstring ws);
	bool SymbolMatches(const uSymbolType symbol);
	bool InSymbolRange(const uSymbolType low, const uSymbolType high);

	uAbstractParserNode* GlobalStatementList();
	uAbstractParserNode* GlobalStatement();

	uFuncParserNode* FuncDef();
	uClassParserNode* ClassDef(bool partial = false);

	uAbstractParserNode* Statement();
	uAbstractParserNode* IfStatement();
	uAbstractParserNode* IfBlock();
	uAbstractParserNode* WhileStatement();
	uAbstractParserNode* ForStatement();
	uAbstractParserNode* ForStatementListInitializer();
	uAbstractParserNode* ForStatementListCondition();
	uAbstractParserNode* ForStatementListLoopCounter();

	uAbstractParserNode* BlockOrStatement();
	uAbstractParserNode* Block();

	uAbstractParserNode* Expression();

	uAbstractParserNode* TernaryIfExpr();
	uAbstractParserNode* TernaryIfBlock();
	
	uAbstractParserNode* Assignment();
	uAbstractParserNode* AugmentedAssignment();
	
	uAbstractParserNode* ExprBoolOr();
	uAbstractParserNode* ExprBoolAnd();
	uAbstractParserNode* ExprBitwiseOr();
	uAbstractParserNode* ExprBitwiseXor();
	uAbstractParserNode* ExprBitwiseAnd();
	uAbstractParserNode* ExprEquality();
	uAbstractParserNode* ExprRelational();
	uAbstractParserNode* ExprBitShift();
	uAbstractParserNode* ExprMathAddSub();
	uAbstractParserNode* ExprMathMulDivMod();
	uAbstractParserNode* ExprPreUnary();
	uAbstractParserNode* ExprPostUnary();
	uAbstractParserNode* ExprCallAndMemberAccess();
	uAbstractParserNode* ArgumentsList();
	uAbstractParserNode* Factor();
};
