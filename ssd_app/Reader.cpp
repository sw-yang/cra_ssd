#include "Reader.h"

Reader::Reader(std::vector<std::string>& args, FileManager* nand, FileManager* result):
	addr_(0), value_(0), nand_(nand), result_(result)
{
	for (std::string& arg : args)
	{
		args_.push_back(arg);
	}
}

void Reader::Run()
{
	Parse();
	Read();
}

bool Reader::isNumber(const std::string& str)
{
	for (char c : str)
	{
		if (!isdigit(c))
		{
			return false;
		}
	}
	return true;
}

unsigned int Reader::hexStringToInt(const std::string& hexStr) {
	unsigned int result;
	std::stringstream ss;
	ss << std::hex << hexStr;
	ss >> result;
	return result;
}

bool Reader::isValidHex(const std::string& str)
{
	std::regex pattern("^0x[0-9a-fA-F]{8}$");
	return std::regex_match(str, pattern);
}

bool Reader::isValidCommand()
{
	if (args_.size() != 1) return false;
	if (!isNumber(args_[0])) return false;
	if (stoi(args_[0]) < 0 || stoi(args_[0]) > 99) return false;

	return true;
}

void Reader::Parse()
{
	if (!isValidCommand())
	{
		throw std::exception("Invalid args");
	}
	addr_ = (unsigned int)std::stoi(args_[0]);
}

void Reader::Read()
{
	uint32_t nand_data[100];

	try
	{
		nand_->ReadBinaryFile(nand_data, sizeof(nand_data));
	}
	catch (std::length_error)
	{
		for (int i = 0; i < 100; ++i)
		{
			nand_data[i] = 0;
		}
		nand_->WriteBinaryFile(nand_data, sizeof(nand_data));
	}
	result_->WriteTextFile(IntToHex(nand_data[addr_]));
}

std::string Reader::IntToHex(uint32_t integer)
{
	ostringstream ss;
	ss << hex << uppercase << integer;
	int zero_fills = 8 - ss.str().length();

	string result = "0x";
	for (int i = 0; i < zero_fills; ++i) {
		result += "0";
	}
	result += ss.str();
	return result;
}
