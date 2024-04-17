#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "CMD.h"

using namespace std;

class SSD
{
public:
	SSD() :
		nand_file_("nand.txt"),
		result_file_("result.txt") {}

	// This constructor is for unit test
	SSD(std::string nand_file, std::string result_file) :
		nand_file_(nand_file),
		result_file_(result_file) {}

	Command Parse(std::string command);
	void Read(uint32_t address);
	void Write(uint32_t address, uint32_t value);

private:
	void ReadNandFile();
	void WriteNandToFile();
	void WriteResultToFile(uint32_t result);
	std::string IntToHex(uint32_t integer);

	uint32_t nand_[100];
	std::string nand_file_;
	std::string result_file_;
};

