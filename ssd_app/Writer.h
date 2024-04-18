#pragma once
#include "iCommand.h"

class Writer : public iCommand
{
public:
	void Run() override;
};

