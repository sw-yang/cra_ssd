#include "SSD.h"
#include "Reader.h"
#include "Writer.h"
#include "Eraser.h"

SSD::SSD(std::string nand_file, std::string result_file)
{
	nand_file_ = new FileManager(nand_file);
	result_file_ = new FileManager(result_file);
}

void SSD::Run(string mode, vector<string> args)
{
	if (mode == "R")
	{
		cmd_ = new Reader(args, nand_file_, result_file_);
	}
	else if (mode == "W")
	{
		cmd_ = new Writer(args, nand_file_);
	}
	else if (mode == "E")
	{
		cmd_ = new Eraser(args, nand_file_);
	}
	else
	{
		throw std::exception("Invalid mode");
	}

	cmd_->Run();
}
