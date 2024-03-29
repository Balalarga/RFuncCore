#include <iostream>
#include "RFLanguage/Lexer/DefaultLexerProcessors.h"
#include "RFLanguage/Lexer/SimpleLexer.h"
#include "RFLanguage/Parser/Parser.h"


struct TokenPrinter
{
	void operator()(const char& ch)
	{
		std::cout << "C: '" << ch << "'\n";
	}
	void operator()(const float& fl)
	{
		std::cout << "N: " << fl << std::endl;
	}
	void operator()(const std::string& str)
	{
		std::cout << "K: " << str << std::endl;
	}
};


void LexerTest()
{
	SimpleLexer lexer;
	lexer.AddLexerProcessor(std::make_unique<SymbolsLexerProcessor>())
		.AddLexerProcessor(std::make_unique<NumberLexerProcessor>())
		.AddLexerProcessor(std::make_unique<KeywordLexerProcessor>());
	std::string sourceCode = R"(
               def main()
               {
                    return 1+2;
               })";
	auto tokens = lexer.Process(sourceCode);

	while (!tokens.empty())
	{
		const auto& token = tokens.front();
		std::visit(TokenPrinter(), token);
		tokens.pop();
	}
	std::cout << "Processed chars: " << lexer.GetProcessedChars() << std::endl;
	if (lexer.GetProcessedChars() < sourceCode.size())
	{
		std::cout << "----------- Rest code ----------\n";
		std::cout << std::string_view(sourceCode).substr(lexer.GetProcessedChars()) << std::endl;
		std::cout << "--------------------------------\n";
	}
	else
	{
		std::cout << sourceCode << std::endl;
	}
}

void ParserTest()
{
	SimpleLexer lexer;
	lexer.AddLexerProcessor(std::make_unique<SymbolsLexerProcessor>())
		.AddLexerProcessor(std::make_unique<NumberLexerProcessor>())
		.AddLexerProcessor(std::make_unique<KeywordLexerProcessor>());
	std::string sourceCode = R"(
               def main()
               {
                    return 1+2;
               })";
	auto tokens = lexer.Process(sourceCode);

	while (!tokens.empty())
	{
		const auto& token = tokens.front();
		std::visit(TokenPrinter(), token);
		tokens.pop();
	}
	Parser parser;
}


int main()
{
	// LexerTest();
	return 0;
}
