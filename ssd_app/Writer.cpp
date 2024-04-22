#include "Writer.h"

WriteCmd::WriteCmd(std::vector<std::string>& args)
{
	for (std::string& arg : args)
	{
		args_.push_back(arg);
	}

	if (!isValidCommand())
	{
		throw std::exception("Invalid args");
	}
	addr_ = (unsigned int)std::stoi(args_[0]);
	value_ = NumUtils::hexStringToInt(args_[1]);
}

std::string WriteCmd::ToString()
{
	std::string ret = "W " + std::to_string(addr_) + " " + args_[1];
	return ret;
}

std::string WriteCmd::GetMode()
{
	return "W";
}

uint32_t WriteCmd::GetAddr()
{
	return addr_;
}

uint32_t WriteCmd::GetValue()
{
	return value_;

}

bool WriteCmd::isValidCommand()
{
	if (args_.size() != 2) return false;
	if (!NumUtils::isNumber(args_[0])) return false;
	if (stoi(args_[0]) < 0 || stoi(args_[0]) > 99) return false;
	if (!NumUtils::isValidHex(args_[1])) return false;

	return true;
}

Writer::Writer(iCmd* cmd, FileManager* nand) :
	cmd_(cmd), addr_(0), value_(0), nand_(nand) {}

void Writer::Run()
{
	addr_ = reinterpret_cast<WriteCmd*>(cmd_)->GetAddr();
	value_ = reinterpret_cast<WriteCmd*>(cmd_)->GetValue();
	Write();
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
