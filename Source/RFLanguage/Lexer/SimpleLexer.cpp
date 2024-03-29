#include "SimpleLexer.h"
#include <sstream>


SimpleLexer& SimpleLexer::AddLexerProcessor(std::unique_ptr<ILexerProcessor>&& obj)
{
	_processors.push_back(std::move(obj));
	return *this;
}

std::queue<LexerToken> SimpleLexer::Process(std::string_view code)
{
	std::queue<LexerToken> tokens;

	bool bContinue = true;
	while (bContinue && _processedPos < code.size())
	{
		bContinue = false;
		for (auto& processor: _processors)
		{
			std::istringstream stream(code.substr(_processedPos).data());
			if (auto token = processor->Process(stream))
			{
				_processedPos += stream.tellg();
				bContinue = true;
				tokens.push(*token);
				break;
			}
		}
	}

	return tokens;
}
