#include <iostream>
#include <memory>
#include "RFLanguage/Parser/LexerObject.h"
#include "RFLanguage/Parser/SimpleLexer.h"


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
	lexer.AddSplitter({'\n', ' ', '\t'});
	auto lexemes = lexer.Process(R"(
               def main()
               {
                    reutrn 1+2;
               })");
	while (!lexemes.empty())
	{
		auto& lexeme = lexemes.front();
		std::cout << lexeme.string << std::endl;
		lexemes.pop();
	}


	return 0;
}
