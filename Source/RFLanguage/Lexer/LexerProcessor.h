#pragma once

#include <optional>
#include <sstream>
#include "Token.h"


class ILexerProcessor
{
public:
	virtual ~ILexerProcessor() = default;

	// Can change stream pos
	// If returns nullopt -> stream will be restored after function execution
	virtual std::optional<LexerToken> Process(std::istringstream& stream) = 0;
};
