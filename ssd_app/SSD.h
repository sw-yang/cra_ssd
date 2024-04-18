#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include "FileManager.h"
#include "iCommand.h"

using namespace std;

class SSD
{
public:
	SSD(string nand_file = "nand.txt", string result_file = "result.txt");

	void Run(string mode, vector<string> args);

private:
	FileManager* nand_file_;
	FileManager* result_file_;
	iCommand* cmd_ = nullptr;
};
