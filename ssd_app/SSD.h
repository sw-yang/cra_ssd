#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "CMD.h"
#include "FileManager.h"

using namespace std;

class SSD
{
public:
	SSD(std::string nand_file = "nand.txt", std::string result_file = "result.txt");

	void Read(uint32_t address);
	void Write(uint32_t address, uint32_t value);
	void Run(Command command_);

private:
	void ReadNandFile();
	void WriteNandToFile();
	void WriteResultToFile(uint32_t result);
	void UpdateNandToFile(uint32_t address);
	std::string IntToHex(uint32_t integer);

	uint32_t nand_[100];
	FileManager* nand_file_;
	FileManager* result_file_;
};
