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
	uint32_t GetAddr();
	uint32_t GetRange();

private:
	std::vector<std::string> args_;

	bool isValidCommand();
};


class Eraser : public iCommand
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
