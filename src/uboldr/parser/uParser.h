#pragma once

#include "../../collections/LinkedList.h"
#include "../lexer/uToken.h"
#include "../lexer/uScanner.h"
#include "../../basic/Types.h"
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
	uAbstractToken* GetEoFToken();
	uAbstractToken* PeekToken();
	void Parse();
	void Parse(LinkedList<uAbstractToken*>* lexerTokens);

	bool ExpectKeywordToken(const std::wstring ws);
	bool ExpectSymbolToken(const uSymbolType symbol);
	bool KeywordMatches(const std::wstring ws);
	bool SymbolMatches(const uSymbolType symbol);
	bool InSymbolRange(const uSymbolType low, const uSymbolType high);

	LinkedList<uAbstractParserNode*>* StatementList();

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
	uAbstractParserNode* FuncDecl();
	uAbstractParserNode* Factor();
};
