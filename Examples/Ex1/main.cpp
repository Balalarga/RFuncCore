#include <iostream>
#include <memory>
#include "RFLanguage/Parser/SimpleLexer.h"
#include "RFLanguage/Parser/LexerObject.h"


class NumberProducer: public ILexerProducer
{
public:
	virtual std::string GetPattern() const
	{
		return "";
	}

	virtual std::unique_ptr<ILexerObject> Produce(std::string_view code)
	{
		return std::make_unique<ILexerObject>();
	}
};


int main()
{
	SimpleLexer lexer;
	lexer.AddProducer<NumberProducer>();


	return 0;
}
