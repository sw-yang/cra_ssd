#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include "FileManager.h"
#include "iCommand.h"
#include "CmdFactory.h"
#include "CmdBuffer.h"

using namespace std;

class SSD
{
public:
	SSD(string nand_file = "nand.txt", string result_file = "result.txt", string buffer_file = "buffer.txt");

	void Run(string mode, vector<string> args);
	CmdBuffer* GetCmdBuffer();

private:
	FileManager* nand_file_;
	FileManager* result_file_;
	CmdBuffer* cmd_buffer_;
	iCommand* cmd_ = nullptr;
};
