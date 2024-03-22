#pragma once

#include <queue>
#include <set>
#include <string_view>


class SimpleLexer
{
public:
	SimpleLexer() = default;
	~SimpleLexer() = default;

	SimpleLexer& AddSplitter(const std::vector<char>& splitters);

	std::queue<std::string> Process(std::string_view code);
	

private:
	std::set<char> _splitters;
};
