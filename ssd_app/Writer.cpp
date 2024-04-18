#include "Writer.h"

WriteCmd::WriteCmd(std::vector<std::string>& args)
{
	for (std::string& arg : args)
	{
		args_.push_back(arg);
	}
}

std::string WriteCmd::ToString()
{
	std::string ret = "W " + std::to_string(GetAddr()) + " " + args_[1];
	return ret;
}

uint32_t WriteCmd::GetAddr()
{
	if (!isValidCommand())
	{
		throw std::exception("Invalid args");
	}
	return (unsigned int)std::stoi(args_[0]);
}

uint32_t WriteCmd::GetValue()
{
	if (!isValidCommand())
	{
		throw std::exception("Invalid args");
	}
	return hexStringToInt(args_[1]);

}

unsigned int WriteCmd::hexStringToInt(const std::string& hexStr)
{
	unsigned int result;
	std::stringstream ss;
	ss << std::hex << hexStr;
	ss >> result;
	return result;
}

bool WriteCmd::isValidHex(const std::string& str)
{
	std::regex pattern("^0x[0-9a-fA-F]{8}$");
	return std::regex_match(str, pattern);
}

bool WriteCmd::isValidCommand()
{
	if (args_.size() != 2) return false;
	if (!isNumber(args_[0])) return false;
	if (stoi(args_[0]) < 0 || stoi(args_[0]) > 99) return false;
	if (!isValidHex(args_[1])) return false;

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
