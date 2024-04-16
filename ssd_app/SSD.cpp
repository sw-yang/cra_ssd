#include <stdexcept>
#include <fstream>
#include <sstream>
#include <iostream>
#include "SSD.h"
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <regex>

using namespace std;

vector<string> split(const string& str) 
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

bool isNumber(const std::string& str) 
{
	for (char c : str) 
	{
		if (!std::isdigit(c)) 
		{
			return false;
		}
	}
	return true;
}

bool isValidHex(const string& str) 
{
	regex pattern("^0x[0-9a-fA-F]{8}$");
	return regex_match(str, pattern);
}

unsigned int hexStringToInt(const std::string& hexStr) {
	unsigned int result;
	stringstream ss;
	ss << std::hex << hexStr;
	ss >> result;
	return result;
}

Command convertToCommand(const vector<string> command) 
{
	return {command[0][0], (unsigned int)stoi(command[1]), hexStringToInt (command[2])};
}

bool isInvalidCommand(const vector<string> command) 
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
	vector<string> result = split(command);
	if (isInvalidCommand(result)) throw std::invalid_argument("Invalid command!");
	return convertToCommand(result);
}

void SSD::Read(uint32_t address)
{
	if (address >= 100)
	{
		throw std::exception("Invalid address");
	}

	ReadNandFile();

	std::ofstream out(result_file_, std::ios::trunc);
	if (!out.is_open())
	{
		throw std::exception("Fail to open result file");
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
	std::ifstream in(nand_file_, std::ios::binary);
	if (!in.is_open())
	{
		throw std::exception("Fail to open nand file");
	}

	// check file size
	in.seekg(0, std::ios::end);
	std::streampos fileSize = in.tellg();
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

std::string SSD::IntToHex(uint32_t integer)
{
	std::ostringstream ss;
	ss << std::hex << integer;
	int zero_fills = 8 - ss.str().length();

	std::string result = "0x";
	for (int i = 0; i < zero_fills; ++i) {
		result += "0";
	}
	result += ss.str();
	return result;
}