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