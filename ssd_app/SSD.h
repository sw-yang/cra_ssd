#pragma once

#include <string>
#include "FileManager.h"

struct Command
{
	char cmd;
	uint32_t address;
	uint32_t value;
	bool operator==(const Command& other) const {
		return cmd == other.cmd && address == other.address && value == other.value;
	}
};

class SSD
{
public:
	SSD(std::string nand_file = "nand.txt", std::string result_file = "result.txt");

	Command Parse(std::string command);
	void Read(uint32_t address);
	void Write(uint32_t address, uint32_t value);

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
