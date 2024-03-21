#pragma once

#include <string_view>


class ILexerObject
{
public:
    virtual std::string GetPattern() const = 0;

    virtual void SetData(std::string_view code)
    {
        _source = code;
    }


    const std::string& GetSource()
    {
        return _source;
    }


private:
    std::string _source;
};
