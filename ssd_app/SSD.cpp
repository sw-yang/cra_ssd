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
		iCmd* cmd = new ReadCmd(args);
		cmd_ = new Reader(cmd, nand_file_, result_file_);
	}
	else if (mode == "W")
	{
		iCmd* cmd = new WriteCmd(args);
		cmd_ = new Writer(cmd, nand_file_);
	}
	else if (mode == "E")
	{
		iCmd* cmd = new EraseCmd(args);
		cmd_ = new Eraser(cmd, nand_file_);
	}
	else
	{
		throw std::exception("Invalid mode");
	}

	cmd_->Run();
}
