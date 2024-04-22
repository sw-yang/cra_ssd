#pragma once
#include <string>
#include <iostream>
#include "NumUtils.h"

class iCmd
{
public:
	virtual std::string ToString() = 0;
	virtual std::string GetMode() = 0;
};

class iCommand
{
public:
	virtual void Run() = 0;
};