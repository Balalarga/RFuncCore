#pragma once
#include <string>


class IAstNode
{
public:
	virtual ~IAstNode() = default;
};

class NumberNode
{
	NumberNode(float value);


	float GetValue() const
	{
		return _value;
	}


private:
	float _value;
};

class VariableNode
{
};

// Including ()?
class UnaryOperationNode
{
};

class BinaryOperationNode
{
};
