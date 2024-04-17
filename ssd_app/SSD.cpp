#include "SSD.h"

Command SSD::Parse(string command)
{
	// Parse command and return Command instance
	// Throw exception for invalid case
	Command cmd(' ', 0, 0);
	vector<string> result = cmd.split(command);
	if (cmd.isInvalidCommand(result)) throw invalid_argument("Invalid command!");
	return cmd.convertToCommand(result);
}

void SSD::Read(uint32_t address)
{
	if (address >= 100)
	{
		throw exception("Invalid address");
	}

	ReadNandFile();

	ofstream out(result_file_, ios::trunc);
	if (!out.is_open())
	{
		throw exception("Fail to open result file");
	}

	out << IntToHex(nand_[address]);
}

void SSD::Write(uint32_t address, uint32_t value)
{
	// Write value to address from nand
	// Throw exception for invalid case
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

string SSD::IntToHex(uint32_t integer)
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
