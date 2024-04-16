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
	// Read address from nand
	// Throw exception for invalid case
}

void SSD::Write(uint32_t address, uint32_t value)
{
	// Write value to address from nand
	// Throw exception for invalid case
}
