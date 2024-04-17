#include "SSD.h"

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
	WriteNandToFile();
}

void SSD::ReadNandFile()
{
	ifstream in(nand_file_, ios::binary);
	if (!in.is_open())
	{
		throw exception("Fail to open nand file");
	}

	// check file size
	in.seekg(0, ios::end);
	streampos fileSize = in.tellg();
	if (fileSize != 400)
	{
		// Invalid nand file data
		for (int i = 0; i < 100; ++i)
		{
			nand_[i] = 0;
		}
	}
	else
	{
		in.seekg(0);
		in.read(reinterpret_cast<char*>(nand_), sizeof(nand_));
	}
}

void SSD::WriteNandToFile()
{
	std::ofstream out(nand_file_, std::ios::trunc);
	if (!out.is_open())
	{
		throw std::exception("Fail to open result file");
	}
	out.write(reinterpret_cast<const char*>(nand_), sizeof(nand_));
}

void SSD::WriteResultToFile(uint32_t result)
{
	std::ofstream out(result_file_, std::ios::trunc);
	if (!out.is_open())
	{
		throw std::exception("Fail to open result file");
	}

	out << IntToHex(result);
}

std::string SSD::IntToHex(uint32_t integer)

{
	ostringstream ss;
	ss << hex << integer;
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
}
