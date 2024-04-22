#include "SSD.h"
#include "Reader.h"
#include "Writer.h"
#include "Eraser.h"

SSD::SSD(std::string nand_file, std::string result_file, std::string buffer_file)
{
	nand_file_ = new FileManager(nand_file);
	result_file_ = new FileManager(result_file);
	buffer_file_ = new FileManager(buffer_file);
	cmd_buffer_ = new CmdBuffer(buffer_file_);
}

void SSD::Run(string mode)
{
	if (mode != "F")
	{
		throw std::exception("Invalid mode");
	}
	Flush();
}

void SSD::Run(string mode, vector<string> args)
{
	CmdFactory factory;
	iCmd* cmd = factory.CreateCmd(mode, args);

	if (mode == "R")
	{
		uint32_t addr = reinterpret_cast<ReadCmd*>(cmd)->GetAddr();
		uint32_t val;
		if (cmd_buffer_->FastRead(addr, val))
		{
			result_file_->WriteTextFile(NumUtils::IntToHex(val));
		}
		else
		{
			iTask* task = new Reader(cmd, nand_file_, result_file_);
			task->Run();
		}
	}
	else if (mode == "W" || mode == "E")
	{
		if (cmd_buffer_->isFull())
		{
			Flush();
		}

		cmd_buffer_->AddCmd(cmd);
	}
}

CmdBuffer* SSD::GetCmdBuffer() {
	return cmd_buffer_;
}

void SSD::Flush()
{
	std::vector<iCmd*> cmds = cmd_buffer_->GetOptimizedCmds();
	for (auto& cmd : cmds)
	{
		std::string mode = cmd->GetMode();
		if (mode == "E")
		{
			iTask* task = new Eraser(cmd, nand_file_);
			task->Run();
		}
		else if (mode == "W")
		{
			iTask* task = new Writer(cmd, nand_file_);
			task->Run();
		}
		else
		{
			throw std::exception("Invalid mode");
		}
	}

	cmd_buffer_->Clear();
}
