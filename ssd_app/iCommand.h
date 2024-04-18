#pragma once
#include <string>
#include <iostream>

class iCmd
{
public:
	virtual std::string ToString() = 0;

protected:
	bool isNumber(const std::string& str)
	{
		for (char c : str)
		{
			if (!isdigit(c))
			{
				return false;
			}
		}
		return true;
	}
};

class iCommand
{
public:
	virtual void Run() = 0;
};