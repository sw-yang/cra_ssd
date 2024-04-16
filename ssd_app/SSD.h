#pragma once

#include <string>
#include <fstream>

using namespace std;

class Command
{
private:
	char cmd;
	uint32_t address;
	uint32_t value;
public:
	Command(char cmd, uint32_t address, uint32_t value) :
		cmd(cmd), address(address), value(value)
	{

	}
	bool operator==(const Command& other) const {
		return cmd == other.cmd && address == other.address && value == other.value;
	}
	vector<string> split(const string& str);
	bool isNumber(const string& str);
	bool isValidHex(const string& str);
	unsigned int hexStringToInt(const string& hexStr);
	Command convertToCommand(const vector<string> command);
	bool isInvalidCommand(const vector<string> command);
};

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
	std::string IntToHex(uint32_t integer);

	uint32_t nand_[100];
	std::string nand_file_;
	std::string result_file_;
};

