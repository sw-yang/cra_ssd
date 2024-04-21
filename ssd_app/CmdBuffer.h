#pragma once
#include <string>
#include <vector>
#include "iCommand.h"

class CmdBuffer
{
public:
	int isFull();
	void Clear();
	void AddCmd(std::string cmd);
	uint32_t FastRead(uint32_t addr);
	std::vector<iCommand*> GetCmdsFastWrite();
};
