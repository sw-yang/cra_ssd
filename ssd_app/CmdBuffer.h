#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "iCommand.h"
#include "FileManager.h"

class CmdBuffer
{
public:
	CmdBuffer(FileManager* buffer) :
		buffer_(buffer) {}

	bool isFull();
	uint32_t GetSize();
	void Clear();
	void AddCmd(iCmd* cmd);
	void GetiCmdList(std::vector<iCmd*>& cmds);
	bool FastRead(uint32_t read_addr, uint32_t& read_val);
	std::vector<iCmd*> GetCmdsFastWrite();

private:
	uint32_t length_;
	const uint32_t MAX_BUFFER_SIZE = 10;
	FileManager* buffer_;
};
