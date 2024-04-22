#include "SSD.h"
#include "Reader.h"
#include "Writer.h"
#include "Eraser.h"

SSD::SSD(std::string nand_file, std::string result_file, std::string buffer_file)
{
	nand_file_ = new FileManager(nand_file);
	result_file_ = new FileManager(result_file);
	cmd_buffer_ = new CmdBuffer(buffer_file);
}

void SSD::Run(string mode, vector<string> args)
{
	CmdFactory factory;
	iCmd* cmd = factory.CreateCmd(mode, args);
	if (mode == "R")
	{
		cmd_ = new Reader(cmd, nand_file_, result_file_);
	}
	else if (mode == "W")
	{
		cmd_ = new Writer(cmd, nand_file_);
	}
	else if (mode == "E")
	{
		cmd_ = new Eraser(cmd, nand_file_);
	}

	cmd_buffer_->AddCmd(cmd);

	cmd_->Run();
}

CmdBuffer* SSD::GetCmdBuffer() {
	return cmd_buffer_;
}