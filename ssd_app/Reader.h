#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <regex>
#include "iCommand.h"
#include "FileManager.h"

using namespace std;

class ReadCmd : public iCmd
{
public:
	ReadCmd(std::vector<std::string>& args);
	std::string ToString();
	std::string GetMode();
	uint32_t GetAddr();

private:
	std::vector<std::string> args_;
	uint32_t addr_;

	bool isValidCommand();
};

class Reader : public iCommand
{
public:
	Reader(iCmd* cmd, FileManager* nand, FileManager* result);
	void Run() override;

private:
	iCmd* cmd_;
	uint32_t addr_;
	uint32_t value_;
	FileManager* nand_;
	FileManager* result_;

	void Read();
};



