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
	const vector<string> args;
	bool isValid;
public:
	Command(vector<string> args) : args(args)
	{
		isValid = !isInvalidCommand(args);
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
	bool operator==(const Command& other) const {
		return cmd == other.cmd && address == other.address && value == other.value;
	}
	vector<string> split(const string& str);
	bool isNumber(const string& str);
	bool isValidHex(const string& str);
	unsigned int hexStringToInt(const string& hexStr);
	Command convertToCommand(const vector<string> command);
	bool isInvalidCommand(const vector<string> command);
};