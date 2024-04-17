#include "CMD.h"

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

bool Command::isInvalidCommand()
{
	if (args.size() == 2 && args[0] == "R")
	{
		if (!isNumber(args[1])) return true;
		if (stoi(args[1]) < 0 || stoi(args[1]) > 99) return true;
	}
	else if (args.size() == 3 && args[0] == "W")
	{
		if (!isNumber(args[1])) return true;
		if (stoi(args[1]) < 0 || stoi(args[1]) > 99) return true;
		if (!isValidHex(args[2])) return true;
	}
	else
	{
		return true;
	}
	return false;
}