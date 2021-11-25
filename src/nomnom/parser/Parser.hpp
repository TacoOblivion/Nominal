#pragma once

#include "../../collections/LinkedList.h"
#include "../lexer/LexerToken.hpp"
#include "../lexer/Lexer.hpp"
#include "../../basic/Types.h"
#include "ParserNodeType.hpp"
#include "ParserNode.hpp"

class uParser
{
	uScanner _lexer;
	LinkedList<AbstractLexerToken*>* _tokens = nullptr;
	AbstractLexerToken* _token = nullptr;
	int64_t _errorCount = 0;


public:
	uParser(std::wstring path) : _lexer(path) {}
	~uParser() {}

	void GetNextToken();
	AbstractLexerToken* GetEoFToken();
	AbstractLexerToken* PeekToken();
	uAbstractParserNode* Parse();
	uAbstractParserNode* Parse(LinkedList<AbstractLexerToken*>* lexerTokens);

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
