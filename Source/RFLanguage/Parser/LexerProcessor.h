#pragma once

#include <string_view>


class ILexerProcessor
{
public:
	~ILexerProcessor() = default;
	virtual void Read(std::istringstream& stream) = 0;
};
