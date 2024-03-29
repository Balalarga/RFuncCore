#include "DefaultLexerProcessors.h"
#include <format>
#include <iostream>
#include <string>


std::set<char> SymbolsLexerProcessor::defaultSymbols = {
	'_',  ' ',	'#', '@', '^', '?', '%', '$', ':', ';', '*', ',', '.',	'=',  '&',	'|',
	'\'', '\"', '/', '[', ']', '{', '}', '(', ')', '!', '-', '+', '\\', '\n', '\t', '\r',
};

SymbolsLexerProcessor::SymbolsLexerProcessor(const std::set<char>& symbols) : _symbols(symbols)
{
}


std::optional<LexerToken> SymbolsLexerProcessor::Process(std::istringstream& stream)
{
	std::cout << "Begin stream pos: " << stream.tellg() << "\n";
	char character = stream.get();
	if (_symbols.contains(character))
	{
		std::cout << "End stream pos: " << stream.tellg() << "\n";
		return character;
	}

	std::cout << "SymbolLexer skip: " << character << "\n";
	std::cout << "End stream pos: " << stream.tellg() << "\n";

	return std::nullopt;
}

std::set<char> KeywordLexerProcessor::defaultAcceptableSymbols = {
	'_',
};

KeywordLexerProcessor::KeywordLexerProcessor(const std::set<char>& acceptableSymbols) :
	_acceptableSymbols(acceptableSymbols)
{
}

std::optional<LexerToken> KeywordLexerProcessor::Process(std::istringstream& stream)
{
	std::stringstream numberStr;
	while (const char character = (char)stream.get())
	{
		bool bIsAlpha = (character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z');
		if (bIsAlpha || _acceptableSymbols.contains(static_cast<char>(character)))
		{
			numberStr << character;
		}
		else
		{
			break;
		}
	}

	if (numberStr.tellp() == 0)
		return std::nullopt;

	return numberStr.str();
}

std::optional<LexerToken> NumberLexerProcessor::Process(std::istringstream& stream)
{
	std::stringstream numberStr;
	bool wasDot = false;

	while (char symbol = stream.get())
	{
		if (std::isdigit(symbol))
		{
			numberStr << symbol;
		}
		else if (symbol == '.' && !wasDot)
		{
			wasDot = true;
			numberStr << symbol;
		}
		else
		{
			break;
		}
	}

	try
	{
		return std::stof(numberStr.str());
	}
	catch (...)
	{
		return std::nullopt;
	}
}
