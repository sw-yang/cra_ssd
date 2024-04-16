#pragma once

#include <string>
#include <fstream>

class SSD
{
public:
	SSD() {}
	void Parse();
	void Run();

private:
	void Read();
	void Write();

	std::string cmd_;
	std::string address_;
	std::string value_;

	uint32_t nand_[100];
	FILE* nand_file_;
};

