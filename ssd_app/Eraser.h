#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <regex>
#include "iCommand.h"
#include "FileManager.h"

class EraseCmd : public iCmd
{
public:
	EraseCmd(std::vector<std::string>& args);
	std::string ToString();
	std::string GetMode();
	uint32_t GetAddr();
	uint32_t GetRange();

private:
	std::vector<std::string> args_;
	uint32_t addr_;
	uint32_t range_;

	bool isValidCommand();
};


class Eraser : public iTask
{
public:
	Eraser(iCmd* cmd, FileManager* nand);
	void Run() override;

private:
	iCmd* cmd_;
	uint32_t addr_;
	uint32_t range_;
	FileManager* nand_;
	
	void Erase();
};
