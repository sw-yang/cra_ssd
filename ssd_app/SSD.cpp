#include <stdexcept>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <regex>
#include "SSD.h"

vector<string> Command::split(const string& str)
{
	vector<string> tokens;
	istringstream tokenStream(str);
	string token;
	while (getline(tokenStream, token, ' ')) 
	{
		if (!token.empty()) 
		{
			tokens.push_back(token);
		}
	}
	return tokens;
}

bool Command::isNumber(const string& str)
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

bool Command::isValidHex(const string& str)
{
	regex pattern("^0x[0-9a-fA-F]{8}$");
	return regex_match(str, pattern);
}

unsigned int Command::hexStringToInt(const string& hexStr) {
	unsigned int result;
	stringstream ss;
	ss << hex << hexStr;
	ss >> result;
	return result;
}

Command Command::convertToCommand(const vector<string> command)
{
	return Command(command[0][0], (unsigned int)stoi(command[1]), hexStringToInt(command[2]));
}

bool Command::isInvalidCommand(const vector<string> command)
{
	if (command.size() != 3) return true; 
	if (!(command[0] == "R" || command[0] == "W")) return true; 
	if (!isNumber(command[1])) return true; 
	if (stoi(command[1]) < 0 || stoi(command[1]) > 99) return true;
	if (!isValidHex(command[2])) return true;
	return false;
}

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
