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
	string cmd;
	uint32_t address;
	uint32_t value;
	vector<string> args;
	bool isValid = false;
public:
	Command(vector<string> args) : args(args)
	{
		initialize();
	}

	Command(const string str)
	{
		args = split(str);
		initialize();
	}

	bool operator==(const Command& other) const 
	{
		return cmd == other.cmd && address == other.address && value == other.value;
	}
	void initialize();
	string getCommand() 
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
	unsigned int hexStringToInt(const string& hexStr);
	bool isValidCommand();
	bool isValidAddressType();
	bool isValidAddress();
	bool isValidValue();

	const int CMD_POSITION = 0;
	const int ADDRESS_POSITION = 1;
	const int VALUE_POSITION = 2;
	const int NUM_OF_ARGS_READ = 2;
	const int NUM_OF_ARGS_WRITE = 3;
};