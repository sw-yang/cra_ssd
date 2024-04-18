#include "CMD.h"

void Command::initialize() {
	isValid = isValidCommand();

	if (isValid)
	{
		cmd = args[CMD_POSITION];
		address = stoi(args[ADDRESS_POSITION]);
		if (args[CMD_POSITION] == "W") value = hexStringToInt(args[VALUE_POSITION]);
	}
}

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

bool Command::isValidAddressType()
{
	for (char c : args[ADDRESS_POSITION])
	{
		if (!isdigit(c))
		{
			return false;
		}
	}
	return true;
}

bool Command::isValidValue()
{
	regex pattern("^0x[0-9a-fA-F]{8}$");
	return regex_match(args[VALUE_POSITION], pattern);
}

unsigned int Command::hexStringToInt(const string& hexStr)
{
	unsigned int result;
	stringstream ss;
	ss << hex << hexStr;
	ss >> result;
	return result;
}

bool Command::isValidAddress()
{
	return isValidAddressType() && (stoi(args[ADDRESS_POSITION]) >= 0) && (stoi(args[ADDRESS_POSITION]) < 100);
}

bool Command::isValidCommand()
{
	if (args.empty()) return false;

	if (args[CMD_POSITION] == "R")
	{
		if (args.size() != NUM_OF_ARGS_READ) return false;
		return isValidAddress();
	}
	else if (args[CMD_POSITION] == "W")
	{
		if (args.size() != NUM_OF_ARGS_WRITE) return false;
		return isValidAddress() && isValidValue();
	}
	else
	{
		return false;
	}
}