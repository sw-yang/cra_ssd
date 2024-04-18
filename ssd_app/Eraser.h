#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <regex>
#include "iCommand.h"
#include "FileManager.h"

class Eraser : public iCommand
{
public:
	Eraser(std::vector<std::string>& args, FileManager* nand);
	void Run() override;

private:
	std::vector<std::string> args_;
	uint32_t addr_;
	uint32_t range_;
	FileManager* nand_;
	
	bool isNumber(const std::string& str);
	bool isValidCommand();
	void Parse();
	void Erase();
};
