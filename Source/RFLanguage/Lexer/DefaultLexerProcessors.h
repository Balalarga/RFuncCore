#pragma once

#include <set>
#include "LexerProcessor.h"


class SymbolsLexerProcessor: public ILexerProcessor
{
public:
	static std::set<char> defaultSymbols;
	SymbolsLexerProcessor(const std::set<char>& symbols = defaultSymbols);

	virtual std::optional<LexerToken> Process(std::istringstream& stream) override;


private:
	std::set<char> _symbols;
};


class KeywordLexerProcessor: public ILexerProcessor
{
public:
	static std::set<char> defaultAcceptableSymbols;
	KeywordLexerProcessor(const std::set<char>& acceptableSymbols = defaultAcceptableSymbols);

	virtual std::optional<LexerToken> Process(std::istringstream& stream) override;


private:
	std::set<char> _acceptableSymbols;
};


class NumberLexerProcessor: public ILexerProcessor
{
public:
	virtual std::optional<LexerToken> Process(std::istringstream& stream) override;


private:
	std::set<char> _symbols;
};
