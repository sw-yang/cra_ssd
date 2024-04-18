#include "SSD.h"
#include "Reader.h"
#include "Writer.h"

SSD::SSD(std::string nand_file, std::string result_file)
{
	nand_file_ = new FileManager(nand_file);
	result_file_ = new FileManager(result_file);
}

void SSD::Read(uint32_t address)
{
	if (address >= 100)
	{
		throw exception("Invalid address");
	}

	ReadNandFile();

	WriteResultToFile(nand_[address]);

}

void SSD::Write(uint32_t address, uint32_t value)
{
	if (address >= 100)
	{
		throw std::exception("Invalid address");
	}

	ReadNandFile();
	nand_[address] = value;
	UpdateNandToFile(address);
}

void SSD::ReadNandFile()
{
	try
	{
		nand_file_->ReadBinaryFile(nand_, sizeof(nand_));
	}
	catch (std::length_error)
	{
		for (int i = 0; i < 100; ++i)
		{
			nand_[i] = 0;
		}
		nand_file_->WriteBinaryFile(nand_, sizeof(nand_));
	}
}

void SSD::WriteNandToFile()
{
	nand_file_->WriteBinaryFile(nand_, sizeof(nand_));
}

void SSD::UpdateNandToFile(uint32_t address)
{
	nand_file_->WriteBinaryFile(&nand_[address], sizeof(uint32_t), address * sizeof(uint32_t));
}

void SSD::WriteResultToFile(uint32_t result)
{
	result_file_->WriteTextFile(IntToHex(result));
}

std::string SSD::IntToHex(uint32_t integer)
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

void SSD::Run(Command command_)
{
#if 1
	if (!command_.getValid()) throw std::invalid_argument("Invalid Command");

	string cmd = command_.getCommand();
	
	if (cmd == "R")
	{
		Read(command_.getAddress());
	}
	else if (cmd == "W")
	{
		Write(command_.getAddress(), command_.getValue());
	}
#else

	// 명령어 R, W 파싱
	char mode = 'R';
	vector<string> tmp_args = { "1", "0x0000000a" };
	FileManager* tmp_nand = new FileManager("nand.txt");

	switch (mode) {
	case 'R':
	case 'r':
		cmd_ = new Reader();
		break;
	case 'W':
	case 'w':
		cmd_ = new Writer(tmp_args, tmp_nand);
		break;
	default:
		// error
		throw std::exception("Invalid address");
	}

	cmd_->Run();

#endif
}
