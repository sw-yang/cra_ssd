#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include "iCommand.h"
#include "FileManager.h"

class WriteCmd : public iCmd
{
public:
	WriteCmd(std::vector<std::string>& args);
	std::string ToString();
	std::string GetMode();
	uint32_t GetAddr();
	uint32_t GetValue();

private:
	std::vector<std::string> args_;

	bool isValidCommand();
};

class Writer : public iCommand
{
public:
	Writer(iCmd* cmd, FileManager* nand);
	void Run() override;

private:
	iCmd* cmd_;
	uint32_t addr_;
	uint32_t value_;
	FileManager* nand_;

	void Write();
};



