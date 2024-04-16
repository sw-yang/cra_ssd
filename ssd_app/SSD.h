#pragma once

#include <string>
#include <fstream>

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
	SSD() {}
	Command Parse(std::string command);
	void Read(uint32_t address);
	void Write(uint32_t address, uint32_t value);

private:
	uint32_t nand_[100];
	FILE* nand_file_;
};

