#pragma once

#include <memory>
#include <queue>
#include <string_view>

#include "RFLanguage/Parser/LexerObject.h"


class Lexer
{
public:
    Lexer() = default;

    std::queue<std::shared_ptr<ILexerObject>> Process(std::string_view code);


private:
};

