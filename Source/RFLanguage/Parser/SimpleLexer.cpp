#include "SimpleLexer.h"


SimpleLexer& SimpleLexer::AddSplitter(const std::vector<char>& splitters)
{
	_splitters.insert(splitters.begin(), splitters.end());
	return *this;
}

std::queue<std::string> SimpleLexer::Process(std::string_view code)
{
	std::queue<std::string> tokens;

	size_t startIdx = 0;
	for (size_t i = startIdx; i < code.size(); ++i)
	{
		if (_splitters.contains(code[i]))
		{
			tokens.push(code.substr(startIdx, i).data());
			startIdx = i + 1;
		}
		
	}

	return tokens;
}
