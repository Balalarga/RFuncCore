#include "Lexer.h"



std::queue<std::shared_ptr<ILexerObject>> Lexer::Process(std::string_view code)
{
    std::queue<std::shared_ptr<ILexerObject>> tokens;

    std::string_view currentSource;
    while(!code.empty())
    {
    }

    return tokens;
}
