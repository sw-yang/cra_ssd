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

class Reader : public iCommand
{
public:
	Reader(std::vector<std::string>& args, FileManager* nand, FileManager* result);
	void Run() override;


private:
	std::vector<std::string> args_;
	uint32_t addr_;
	uint32_t value_;
	FileManager* nand_;
	FileManager* result_;

	bool isNumber(const std::string& str);
	bool isValidCommand();
	void Parse();
	void Read();
	string IntToHex(uint32_t integer);
};



