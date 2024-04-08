#pragma once
#include <memory>
#include <queue>
#include "ParserRules.h"
#include "RFLanguage/Lexer/Token.h"


class Parser
{
public:
	void AddRule(std::unique_ptr<ParserRule>&& Rule);
	void Produce(std::queue<LexerToken>&& tokenQueue);
};
