#include "Writer.h"

Writer::Writer(std::vector<std::string>& args, FileManager* nand) :
	addr_(0), value_(0), nand_(nand)
{
	for (std::string& arg : args)
	{
		args_.push_back(arg);
	}
}

void Writer::Run()
{
	Parse();
	Write();
}

bool Writer::isNumber(const std::string& str)
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

unsigned int Writer::hexStringToInt(const std::string& hexStr) {
	unsigned int result;
	std::stringstream ss;
	ss << std::hex << hexStr;
	ss >> result;
	return result;
}

bool Writer::isValidHex(const std::string& str)
{
	std::regex pattern("^0x[0-9a-fA-F]{8}$");
	return std::regex_match(str, pattern);
}

bool Writer::isValidCommand()
{
	if (args_.size() != 2) return false;
	if (!isNumber(args_[0])) return false;
	if (stoi(args_[0]) < 0 || stoi(args_[0]) > 99) return false;
	if (!isValidHex(args_[1])) return false;

	return true;
}

void Writer::Parse()
{
	if (!isValidCommand())
	{
		throw std::exception("Invalid args");
	}
	addr_ = (unsigned int)std::stoi(args_[0]);
	value_ = hexStringToInt(args_[1]);
}

void Writer::Write()
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
	}

	nand_data[addr_] = value_;
	nand_->WriteBinaryFile(nand_data, sizeof(nand_data));
}
