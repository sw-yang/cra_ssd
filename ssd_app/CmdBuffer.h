#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "iCommand.h"

class CmdBuffer
{
public:
	CmdBuffer(std::string file_name) :
		file_name_(file_name) {}

	bool isFull();
	uint32_t GetSize();
	void Clear();
	void AddCmd(iCmd* cmd);
	void GetiCmdList(std::vector<iCmd*>& cmds);
	bool FastRead(uint32_t read_addr, uint32_t& read_val);
	std::vector<iCmd*> GetCmdsFastWrite();

private:
	void CreateEmptyFile();
	std::fstream OpenFile(bool isTrunc = false);
	std::vector<std::string> SplitStringToVector(const std::string& str);
	uint32_t length_;
	const uint32_t MAX_BUFFER_SIZE = 10;
	std::string file_name_;
};
