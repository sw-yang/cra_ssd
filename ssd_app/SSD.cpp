#include <stdexcept>
#include <fstream>
#include <sstream>
#include <iostream>
#include "SSD.h"

Command SSD::Parse(std::string command)
{
	// Parse command and return Command instance
	// Throw exception for invalid case
	return {};
}

void SSD::Read(uint32_t address)
{
	if (address >= 100)
	{
		throw std::exception("Invalid address");
	}

	std::ifstream in(nand_file_, std::ios::binary);
	if (!in.is_open())
	{
		throw std::exception("Fail to open nand file");
	}

	std::ofstream out(result_file_, std::ios::trunc);
	if (!out.is_open())
	{
		throw std::exception("Fail to open result file");
	}

	// check file size
	in.seekg(0, std::ios::end);
	std::streampos fileSize = in.tellg();

	if (fileSize != 400)
	{
		// Invalid nand file data
		for (int i = 0; i < 100; ++i)
		{
			nand_[i] = 0;
		}
	}
	else
	{
		in.seekg(0);
		in.read(reinterpret_cast<char*>(nand_), sizeof(nand_));

	}

	std::ostringstream ss;
	ss << std::hex << nand_[address];
	int zero_fills = 8 - ss.str().length();

	std::string result = "0x";
	for (int i = 0; i < zero_fills; ++i) {
		result += "0";
	}
	result += ss.str();

	out << result;
}

void SSD::Write(uint32_t address, uint32_t value)
{
	// Write value to address from nand
	// Throw exception for invalid case
}
