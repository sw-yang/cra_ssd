#pragma once
#include <string>
#include <sstream>
#include <regex>

static class NumUtils
{
public:
	static bool isNumber(const std::string& str)
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

	static std::string IntToHex(uint32_t integer)
	{
		std::ostringstream ss;
		ss << std::hex << std::uppercase << integer;
		int zero_fills = 8 - ss.str().length();

		std::string result = "0x";
		for (int i = 0; i < zero_fills; ++i) {
			result += "0";
		}
		result += ss.str();
		return result;
	}

	static unsigned int hexStringToInt(const std::string& hexStr)
	{
		unsigned int result;
		std::stringstream ss;
		ss << std::hex << hexStr;
		ss >> result;
		return result;
	}

	static bool isValidHex(const std::string& str)
	{
		std::regex pattern("^0x[0-9a-fA-F]{8}$");
		return std::regex_match(str, pattern);
	}
};
