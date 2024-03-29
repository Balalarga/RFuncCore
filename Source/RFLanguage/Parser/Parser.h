#pragma once
#include <queue>
#include "RFLanguage/Lexer/Token.h"


class Parser
{
public:
	void Produce(std::queue<LexerToken>&& tokenQueue);
};
