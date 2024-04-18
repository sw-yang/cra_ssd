#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <regex>
#include "iCommand.h"
#include "FileManager.h"

using namespace std;

class ReadCmd : public iCmd
{
public:
	ReadCmd(std::vector<std::string>& args);
	std::string ToString();
	uint32_t GetAddr();

private:
	std::vector<std::string> args_;

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
	string IntToHex(uint32_t integer);
};



