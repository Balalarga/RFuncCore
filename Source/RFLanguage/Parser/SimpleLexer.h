#pragma once

#include <cstdint>
#include <memory>
#include <queue>
#include <string_view>
#include "LexerProcessor.h"


struct Lexeme
{
	std::string string;
	uint32_t line;
	uint32_t column;
};

class SimpleLexer
{
public:
	SimpleLexer() = default;
	~SimpleLexer() = default;

	SimpleLexer& AddLexerProcessor(std::unique_ptr<ILexerProcessor>&& obj);

	std::queue<Lexeme> Process(std::string_view code);


private:
	std::vector<std::unique_ptr<ILexerProcessor>> _processors;
};
