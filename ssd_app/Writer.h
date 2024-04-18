#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <regex>
#include "iCommand.h"
#include "FileManager.h"

class Writer : public iCommand
{
public:
	Writer(std::vector<std::string>& args, FileManager* nand);
	void Run() override;

private:
	std::vector<std::string> args_;
	uint32_t addr_;
	uint32_t value_;
	FileManager* nand_;

	bool isNumber(const std::string& str);
	unsigned int hexStringToInt(const std::string& hexStr);
	bool isValidHex(const std::string& str);
	bool isValidCommand();
	void Parse();
	void Write();
};



