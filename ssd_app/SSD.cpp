#include <stdexcept>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <regex>
#include "SSD.h"

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


SSD::SSD(std::string nand_file, std::string result_file)
{
	nand_file_ = new FileManager(nand_file);
	result_file_ = new FileManager(result_file);
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
