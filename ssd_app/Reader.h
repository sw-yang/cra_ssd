#pragma once
#include "iCommand.h"

class Reader : public iCommand
{
public:
	void Run() override;
};

