#include "Reader.h"

ReadCmd::ReadCmd(std::vector<std::string>& args)
{
	for (std::string& arg : args)
	{
		args_.push_back(arg);
	}
}

std::string ReadCmd::ToString()
{
	std::string ret = "R " + std::to_string(GetAddr());
	return ret;
}

std::string ReadCmd::GetMode()
{
	return "R";
}

uint32_t ReadCmd::GetAddr()
{
	if (!isValidCommand())
	{
		throw std::exception("Invalid args");
	}
	return (unsigned int)std::stoi(args_[0]);
}

bool ReadCmd::isValidCommand()
{
	if (args_.size() != 1) return false;
	if (!NumUtils::isNumber(args_[0])) return false;
	if (stoi(args_[0]) < 0 || stoi(args_[0]) > 99) return false;

	return true;
}


Reader::Reader(iCmd* cmd, FileManager* nand, FileManager* result) :
	cmd_(cmd), addr_(0), value_(0), nand_(nand), result_(result) {}

void Reader::Run()
{
	addr_ = reinterpret_cast<ReadCmd*>(cmd_)->GetAddr();
	Read();
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
	result_->WriteTextFile(NumUtils::IntToHex(nand_data[addr_]));
}
