#include "SimpleLexer.h"
#include <sstream>


SimpleLexer& SimpleLexer::AddLexerProcessor(std::unique_ptr<ILexerProcessor>&& obj)
{
	_processors.push_back(std::move(obj));
	return *this;
}

std::queue<Lexeme> SimpleLexer::Process(std::string_view code)
{
	std::queue<Lexeme> tokens;

	std::istringstream stream(code.data());

	for (auto& processor: _processors)
		processor->Read(stream);

	return tokens;
}
