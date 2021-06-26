
#include "uParserNode.h"

void uAbstractParserNode::PrintDebug(int32_t depth, int32_t side)
{
	LinkedList<uAbstractParserNode*>* stmts = nullptr;
	LinkedListItem<uAbstractParserNode*>* stmtNode = nullptr;

	for (int32_t i = 0; i < depth; ++i)
		std::cout << '\t';

	if (side == 0) std::cout << "C: ";
	else if (side == 1) std::cout << "L: ";
	else if (side == 2) std::cout << "R: ";
	else if (side == 3) std::cout << "S: ";

	std::cout << (int32_t)(Type) << '(' << TypeToName() << ')';

	if (Type == uParserNodeType::Identifier)
		std::cout << " - Value: " << WStrToStr(((uParserNode<std::wstring>*)this)->Data);
	else if (Type == uParserNodeType::Integer)
		std::cout << " - Value: " << ((uParserNode<int64_t>*)this)->Data;
	
	std::cout << std::endl;

	if (Type == uParserNodeType::StatementList)
	{
		stmts = ((uParserNode<LinkedList<uAbstractParserNode*>*>*)this)->Data;
		stmtNode = stmts->FirstItem();

		while (stmtNode != nullptr && stmtNode->Value != nullptr)
		{
			stmtNode->Value->PrintDebug(depth + 1, 3);
			stmtNode = stmtNode->NextItem;
		}
	}
	else
	{
		if (Left != nullptr)
			Left->PrintDebug(depth + 1, 1);

		if (Right != nullptr)
			Right->PrintDebug(depth + 1, 2);
	}
}

std::string uAbstractParserNode::TypeToName()
{
	switch (Type)
	{
	case uParserNodeType::Unknown: return "Unknown";
	case uParserNodeType::Add: return "Add";
	case uParserNodeType::Plus: return "Plus";
	case uParserNodeType::Subtract: return "Subtract";
	case uParserNodeType::Negate: return "Negate";
	case uParserNodeType::Multiply: return "Multiply";
	case uParserNodeType::Divide: return "Divide";
	case uParserNodeType::Modulus: return "Modulus";
	case uParserNodeType::PreIncrement: return "PreIncrement";
	case uParserNodeType::PostIncrement: return "PostIncrement";
	case uParserNodeType::PreDecrement: return "PreDecrement";
	case uParserNodeType::PostDecrement: return "PostDecrement";
	case uParserNodeType::ArithmeticRightShift: return "ArithmeticRightShift";
	case uParserNodeType::BoolAnd: return "BoolAnd";
	case uParserNodeType::BoolOr: return "BoolOr";
	case uParserNodeType::BoolNot: return "BoolNot";
	case uParserNodeType::BoolXor: return "BoolXor";
	case uParserNodeType::BoolImplication: return "BoolImplication";
	case uParserNodeType::EqualTo: return "EqualTo";
	case uParserNodeType::NotEqualTo: return "NotEqualTo";
	case uParserNodeType::IdenticalTo: return "IdenticalTo";
	case uParserNodeType::NotIdenticalTo: return "NotIdenticalTo";
	case uParserNodeType::GreaterThan: return "GreaterThan";
	case uParserNodeType::GreaterThanEqual: return "GreaterThanEqual";
	case uParserNodeType::LessThan: return "LessThan";
	case uParserNodeType::LessThanEqual: return "LessThanEqual";
	case uParserNodeType::BitwiseAnd: return "BitwiseAnd";
	case uParserNodeType::BitwiseOr: return "BitwiseOr";
	case uParserNodeType::BitwiseNot: return "BitwiseNot";
	case uParserNodeType::BitwiseXor: return "BitwiseXor";
	case uParserNodeType::BitwiseImplication: return "BitwiseImplication";
	case uParserNodeType::BitwiseLeftShift: return "BitwiseLeftShift";
	case uParserNodeType::BitwiseRightShift: return "BitwiseRightShift";
	case uParserNodeType::Reference: return "Reference";
	case uParserNodeType::Block: return "Block";
	case uParserNodeType::Integer: return "Integer";
	case uParserNodeType::Float: return "Float";
	case uParserNodeType::Identifier: return "Identifier";
	case uParserNodeType::String: return "String";
	case uParserNodeType::Null: return "Null";
	case uParserNodeType::StatementList: return "StatementList";
	case uParserNodeType::IfStatement: return "IfStatement";
	case uParserNodeType::WhileStatement: return "WhileStatement";
	case uParserNodeType::ForStatement: return "ForStatement";
	case uParserNodeType::DeleteStatement: return "DeleteStatement";
	case uParserNodeType::Assignment: return "Assignment";
	case uParserNodeType::IfBlock: return "IfBlock";
	case uParserNodeType::TernaryIfBlock: return "TernaryIfBlock";
	case uParserNodeType::ForStatementListInitializer: return "ForStatementListInitializer";
	case uParserNodeType::ForStatementListCondition: return "ForStatementListCondition";
	case uParserNodeType::ForStatementListLoopCounter: return "ForStatementListLoopCounter";
	case uParserNodeType::FnCall: return "FnCall";
	case uParserNodeType::BracketAccess: return "BracketAccess";
	case uParserNodeType::MemberAccess: return "MemberAccess";
	}
	return "Undefined";
}

uGlobalParserStatementListNode::uGlobalParserStatementListNode()
{
	Type = uParserNodeType::StatementList;
}

uFuncParserNode::uFuncParserNode()
{
	Type = uParserNodeType::FnDef;
}

uClassParserNode::uClassParserNode()
{
	Type = uParserNodeType::ClassDef;
}

