#pragma once
#include <string>
#include <vector>
#include "iCommand.h"

class CmdBuffer
{
public:
	int isFull();
	void Clear();
	void AddCmd(std::string mode, std::vector<std::string> args);
	bool FastRead(const uint32_t addr, uint32_t& value);
	std::vector<iCommand*> GetCmdsFastWrite();
};
