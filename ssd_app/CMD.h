#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <regex>
#include <stdexcept>

using namespace std;

class Command
{
private:
	char cmd;
	uint32_t address;
	uint32_t value;
	vector<string> args;
	bool isValid = false;
public:
	Command(vector<string> args) : args(args)
	{
		isValid = !isInvalidCommand();
		if (isValid)
		{
			cmd = args[0][0];
			address = (unsigned int)stoi(args[1]);
			value = hexStringToInt(args[2]);
		}
	}
	Command(const string str)
	{
		args = split(str);
		isValid = !isInvalidCommand();
		if (isValid)
		{
			cmd = args[0][0];
			address = (unsigned int)stoi(args[1]);
			value = hexStringToInt(args[2]);
		}
	}
	Command(char cmd, uint32_t address, uint32_t value) : 
		cmd(cmd), address(address), value(value)
	{

	}
	bool operator==(const Command& other) const 
	{
		return cmd == other.cmd && address == other.address && value == other.value;
	}
	char getCommand() 
	{
		return cmd;
	}
	uint32_t getAddress() 
	{
		return address;
	}
	uint32_t getValue() 
	{
		return value;
	}
	bool getValid() 
	{
		return isValid;
	}
	vector<string> split(const string& str);
	bool isNumber(const string& str);
	bool isValidHex(const string& str);
	unsigned int hexStringToInt(const string& hexStr);
	bool isInvalidCommand();
};