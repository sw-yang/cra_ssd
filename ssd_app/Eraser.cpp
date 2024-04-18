#include "Eraser.h"

EraseCmd::EraseCmd(std::vector<std::string>& args)
{
	for (std::string& arg : args)
	{
		args_.push_back(arg);
	}
}

std::string EraseCmd::ToString()
{
	std::string ret = "E " + std::to_string(GetAddr()) + " " + std::to_string(GetRange());
	return ret;
}

uint32_t EraseCmd::GetAddr()
{
	if (!isValidCommand())
	{
		throw std::exception("Invalid args");
	}
	return (unsigned int)std::stoi(args_[0]);
}

uint32_t EraseCmd::GetRange()
{
	if (!isValidCommand())
	{
		throw std::exception("Invalid args");
	}
	return (unsigned int)std::stoi(args_[1]);
}

bool EraseCmd::isValidCommand()
{
	if (args_.size() != 2) return false;
	if (!isNumber(args_[0]) || !isNumber(args_[1])) return false;
	if (stoi(args_[0]) < 0 || stoi(args_[0]) > 99) return false;
	if (stoi(args_[1]) < 1 || stoi(args_[1]) > 10) return false;
	if (stoi(args_[0]) + stoi(args_[1]) > 99) return false;

	return true;
}


Eraser::Eraser(iCmd* cmd, FileManager* nand) :
	cmd_(cmd), addr_(0), range_(0), nand_(nand) {}

void Eraser::Run()
{
	addr_ = reinterpret_cast<EraseCmd*>(cmd_)->GetAddr();
	range_ = reinterpret_cast<EraseCmd*>(cmd_)->GetRange();
	Erase();
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
