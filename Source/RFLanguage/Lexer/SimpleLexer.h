#pragma once

#include <memory>
#include <queue>
#include <string_view>
#include "LexerProcessor.h"


class SimpleLexer
{
public:
	SimpleLexer() = default;
	~SimpleLexer() = default;

	SimpleLexer& AddLexerProcessor(std::unique_ptr<ILexerProcessor>&& obj);

	std::queue<LexerToken> Process(std::string_view code);
	size_t GetProcessedChars() const
	{
		return _processedPos;
	}


private:
	std::vector<std::unique_ptr<ILexerProcessor>> _processors;
	size_t _processedPos = 0;
};
