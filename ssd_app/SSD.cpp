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
	if (!in.is_open()) {
		throw std::exception("Fail to open nand file");
	}

	std::ofstream out(result_file_, std::ios::trunc);
	if (!out.is_open()) {
		throw std::exception("Fail to open result file");
	}

	out << "0x00000000";
}

void SSD::Write(uint32_t address, uint32_t value)
{
	// Write value to address from nand
	// Throw exception for invalid case
}
