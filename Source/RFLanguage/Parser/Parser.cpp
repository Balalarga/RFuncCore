#include "Parser.h"


void Parser::Produce(std::queue<LexerToken>&& tokenQueue)
{
	while (!tokenQueue.empty())
	{
		auto token = tokenQueue.front();


		tokenQueue.pop();
	}
}
