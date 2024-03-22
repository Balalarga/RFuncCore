#pragma once

#include <memory>
#include <string_view>


class ILexerObject
{
public:
	~ILexerObject() = default;
};


class ILexerProducer
{
public:
	~ILexerProducer() = default;

	virtual std::string GetPattern() const = 0;
	virtual std::unique_ptr<ILexerObject> Produce(std::string_view code) = 0;
};
