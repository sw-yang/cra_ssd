#include "Eraser.h"

Eraser::Eraser(std::vector<std::string>& args, FileManager* nand) :
	addr_(0), range_(0), nand_(nand)
{
	for (std::string& arg : args)
	{
		args_.push_back(arg);
	}
}

void Eraser::Run()
{
	Parse();
	Erase();
}

bool Eraser::isNumber(const std::string& str)
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

bool Eraser::isValidCommand()
{
	if (args_.size() != 2) return false;
	if (!isNumber(args_[0]) || !isNumber(args_[1])) return false;
	if (stoi(args_[0]) < 0 || stoi(args_[0]) > 99) return false;
	if (stoi(args_[1]) < 1 || stoi(args_[1]) > 10) return false;
	if (stoi(args_[0]) + stoi(args_[1]) > 99) return false;

	return true;
}

void Eraser::Parse()
{
	if (!isValidCommand())
	{
		throw std::exception("Invalid args");
	}
	addr_ = (unsigned int)std::stoi(args_[0]);
	range_ = (unsigned int)std::stoi(args_[1]);
}

void Eraser::Erase()
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

	for (uint32_t i = 0; i < range_; ++i)
	{
		nand_data[addr_ + i] = 0;
	}
	nand_->WriteBinaryFile(nand_data, sizeof(nand_data));
}
