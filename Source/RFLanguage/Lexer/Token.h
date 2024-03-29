#pragma once
#include <string>
#include <variant>


using LexerToken = std::variant<char, std::string, float>;
