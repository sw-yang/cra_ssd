#include "CmdBuffer.h"
#include "CmdFactory.h"
#include "Eraser.h"
#include "Writer.h"

#include <map>

bool CmdBuffer::isFull()
{
	return length_ == MAX_BUFFER_SIZE;
}

uint32_t CmdBuffer::GetSize()
{
	return length_;
}

void CmdBuffer::Clear()
{
	std::fstream file = OpenFile(true);
	SetSize(0);
}

void CmdBuffer::SetSize(uint32_t length)
{
	length_ = length;
}

void CmdBuffer::AddCmd(iCmd* cmd)
{
	if (isFull()) throw std::length_error("There is no space in command buffer");

	std::string line = cmd->ToString();

	std::fstream file = OpenFile();

	file.seekp(0, std::ios::end);
	file << line << "\n";

	file.close();

	SetSize(GetSize() + 1);
}

void CmdBuffer::GetCmd(std::vector<std::string>& cmds)
{
	std::fstream file = OpenFile();

	std::string line;
	while (std::getline(file, line))
	{
		cmds.push_back(line);
	}

	file.close();
}

void CmdBuffer::GetCmdList(std::vector<std::vector<std::string>>& cmds)
{
	std::fstream file = OpenFile();

	std::string line;
	while (std::getline(file, line))
	{
		cmds.push_back(SplitStringToVector(line));
	}

	file.close();
}

void CmdBuffer::GetiCmdList(std::vector<iCmd*>& cmds)
{
	std::fstream file = OpenFile();

	CmdFactory factory;
	std::string line;

	while (std::getline(file, line))
	{
		cmds.push_back(factory.CreateCmd(line));
	}

	file.close();
}

std::fstream CmdBuffer::OpenFile(bool isTrunc)
{
	std::fstream file;
	int openmode = std::ios::in | std::ios::out;

	if (isTrunc)
	{
		openmode |= std::ios::trunc;
	}

	file.open(file_name_, openmode);
	if (!file.is_open())
	{
		CreateEmptyFile();
		file.open(file_name_, openmode);
	}

	return std::move(file);
}

void CmdBuffer::CreateEmptyFile()
{
	std::ofstream out(file_name_);
	if (!out.is_open())
	{
		throw std::exception("Fail to open result file");
	}
}

std::vector<std::string> CmdBuffer::SplitStringToVector(const std::string& str) {
	std::vector<std::string> tokens;
	std::istringstream tokenStream(str);
	std::string token;
	while (getline(tokenStream, token, ' '))
	{
		if (!token.empty())
		{
			tokens.push_back(token);
		}
	}
	return tokens;
}

bool CmdBuffer::FastRead(uint32_t read_addr, uint32_t& read_val)
{
	std::vector<iCmd*> cmds;
	GetiCmdList(cmds);

	std::map<uint32_t, uint32_t> data_map;

	for (auto& cmd : cmds)
	{
		if (cmd->GetMode() == "W")
		{
			WriteCmd* write_cmd = reinterpret_cast<WriteCmd*>(cmd);
			uint32_t addr = write_cmd->GetAddr();
			uint32_t val = write_cmd->GetValue();

			data_map[addr] = val;
		}
		else if (cmd->GetMode() == "E")
		{
			EraseCmd* erase_cmd = reinterpret_cast<EraseCmd*>(cmd);
			uint32_t addr = erase_cmd->GetAddr();
			uint32_t range = erase_cmd->GetRange();

			for (uint32_t i = 0; i < range; ++i)
			{
				data_map[addr + i] = 0;
			}
		}
	}

	if (data_map.find(read_addr) != data_map.end())
	{
		read_val = data_map[read_addr];
		return true;
	}
	else
	{
		return false;
	}
}

std::vector<iCmd*> CmdBuffer::GetCmdsFastWrite()
{
	std::vector<iCmd*> cmds;
	GetiCmdList(cmds);

	std::vector<iCmd*> ret;
	std::vector<iCmd*> fast_cmds_write;

	int erase_area[101] = { 0, };

	for (int i = 0; i < cmds.size(); ++i)
	{
		if (cmds[i]->GetMode() == "W")
		{
			uint32_t write_addr = reinterpret_cast<WriteCmd*>(cmds[i])->GetAddr();

			// Check write cmd could be removed
			for (int j = i + 1; j < cmds.size(); ++j)
			{
				if (cmds[j]->GetMode() == "W")
				{
					if (reinterpret_cast<WriteCmd*>(cmds[j])->GetAddr() == write_addr)
					{
						break;
					}
				}
				else if (cmds[j]->GetMode() == "E")
				{
					uint32_t erase_addr = reinterpret_cast<EraseCmd*>(cmds[j])->GetAddr();
					uint32_t erase_range = reinterpret_cast<EraseCmd*>(cmds[j])->GetRange();
					if (erase_addr <= write_addr && write_addr < erase_addr + erase_range)
					{
						break;
					}
				}
			}

			// Write cmd needed
			fast_cmds_write.push_back(cmds[i]);
		}
		else if (cmds[i]->GetMode() == "E")
		{
			uint32_t start = reinterpret_cast<EraseCmd*>(cmds[i])->GetAddr();
			uint32_t range = reinterpret_cast<EraseCmd*>(cmds[i])->GetRange();

			for (int j = 0; j < range; ++j)
			{
				erase_area[start + j] = 1;
			}
		}
	}

	uint32_t erase_start;
	bool is_erase = false;
	for (int i = 0; i < 100; ++i)
	{
		if (erase_area[i] == 1)
		{
			if (is_erase == false)
			{
				is_erase = true;
				erase_start = i;
			}
			else
			{
				uint32_t erase_range = i - erase_start;
				if (erase_range == 10)
				{
					is_erase = false;
					std::vector<std::string> erase_args;
					erase_args.push_back(std::to_string(erase_start));
					erase_args.push_back(std::to_string(erase_range));
					ret.push_back(new EraseCmd(erase_args));
				}
			}
		}
		else if (erase_area[i] != 1)
		{
			if (is_erase == true)
			{
				is_erase = false;
				uint32_t erase_range = i - erase_start;
				std::vector<std::string> erase_args;
				erase_args.push_back(std::to_string(erase_start));
				erase_args.push_back(std::to_string(erase_range));
				ret.push_back(new EraseCmd(erase_args));
			}
			else
			{
				continue;
			}
		}
	}

	for (auto& cmd : fast_cmds_write)
	{
		ret.push_back(cmd);
	}

	return ret;
}