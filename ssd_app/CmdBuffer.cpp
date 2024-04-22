#include "CmdBuffer.h"
#include "CmdFactory.h"
#include "Eraser.h"
#include "Writer.h"

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

void CmdBuffer::InsertWriteCmd(std::map<uint32_t, uint32_t>& m, iCmd* cmd)
{
	WriteCmd* write_cmd = reinterpret_cast<WriteCmd*>(cmd);
	uint32_t addr = write_cmd->GetAddr();
	uint32_t val = write_cmd->GetValue();

	m[addr] = val;
}

void CmdBuffer::InsertEraseCmd(std::map<uint32_t, uint32_t>& m, iCmd* cmd)
{
	EraseCmd* erase_cmd = reinterpret_cast<EraseCmd*>(cmd);
	uint32_t addr = erase_cmd->GetAddr();
	uint32_t range = erase_cmd->GetRange();

	for (uint32_t i = 0; i < range; ++i)
	{
		m[addr + i] = 0;
	}
}

std::map<uint32_t, uint32_t> CmdBuffer::CreateDataMap(const std::vector<iCmd*>& cmds)
{
	std::map<uint32_t, uint32_t> ret;

	for (auto& cmd : cmds)
	{
		if (cmd->GetMode() == "W")
		{
			InsertWriteCmd(ret, cmd);
		}
		else if (cmd->GetMode() == "E")
		{
			InsertEraseCmd(ret, cmd);
		}
	}

	return ret;
}

bool CmdBuffer::FastRead(uint32_t read_addr, uint32_t& read_val)
{
	std::vector<iCmd*> cmds;
	GetiCmdList(cmds);

	std::map<uint32_t, uint32_t> data_map = CreateDataMap(cmds);

	if (data_map.find(read_addr) == data_map.end())
	{
		return false;
	}

	read_val = data_map[read_addr];
	return true;
}


bool CmdBuffer::IsOverWritten(const uint32_t addr, iCmd* cmd)
{
	if (cmd->GetMode() != "W")
	{
		return false;
	}

	if (reinterpret_cast<WriteCmd*>(cmd)->GetAddr() == addr)
	{
		return true;
	}

	return false;
}

bool CmdBuffer::IsErased(const uint32_t addr, iCmd* cmd)
{
	if (cmd->GetMode() != "E")
	{
		return false;
	}

	uint32_t erase_addr = reinterpret_cast<EraseCmd*>(cmd)->GetAddr();
	uint32_t erase_range = reinterpret_cast<EraseCmd*>(cmd)->GetRange();
	if (erase_addr <= addr && addr < erase_addr + erase_range)
	{
		return true;
	}

	return false;
}

bool CmdBuffer::isRemovableWriteCmd(const std::vector<iCmd*>& cmds, const int cmd_idx)
{
	bool is_removable = false;
	
	uint32_t write_addr = reinterpret_cast<WriteCmd*>(cmds[cmd_idx])->GetAddr();
	for (int i = cmd_idx + 1; i < cmds.size(); ++i)
	{
		if (IsOverWritten(write_addr, cmds[i]) || IsErased(write_addr, cmds[i]))
		{
			is_removable = true;
			break;
		}
	}

	return is_removable;
}

std::vector<iCmd*> CmdBuffer::OptimizeWriteCmds(const std::vector<iCmd*>& cmds)
{
	std::vector<iCmd*> optimized_write_cmds;

	for (int i = 0; i < cmds.size(); ++i)
	{
		if (cmds[i]->GetMode() != "W")
		{
			continue;
		}

		if (!isRemovableWriteCmd(cmds, i))
		{
			optimized_write_cmds.push_back(cmds[i]);
		}
	}

	return optimized_write_cmds;
}

void CmdBuffer::AddEraseArea(std::set<int>& erase_addrs, iCmd* cmd)
{
	uint32_t start = reinterpret_cast<EraseCmd*>(cmd)->GetAddr();
	uint32_t range = reinterpret_cast<EraseCmd*>(cmd)->GetRange();
	for (int i = 0; i < range; ++i)
	{
		erase_addrs.insert(start + i);
	}
}

void CmdBuffer::InsertEraseCmd(std::vector<iCmd*>& erase_cmds, int start, int last)
{
	int range = last - start + 1;

	std::vector<std::string> erase_args;
	erase_args.push_back(std::to_string(start));
	erase_args.push_back(std::to_string(range));

	erase_cmds.push_back(new EraseCmd(erase_args));
}

std::vector<iCmd*> CmdBuffer::GetMergedEraseCmds(std::set<int>& erase_addrs)
{
	std::vector<iCmd*> optimized_erase_cmds;

	int start = -1;
	int last = -1;
	for (auto& cur : erase_addrs)
	{
		if (start == -1)
		{	// initial addr
			start = cur;
			last = cur;
			continue;
		}

		if (cur == last + 1)
		{	// continueous addr
			last = cur;
			if (last - start == 9)
			{
				InsertEraseCmd(optimized_erase_cmds, start, last);
				start = -1;
				last = -1;
			}
		}
		else
		{	// separated addr
			InsertEraseCmd(optimized_erase_cmds, start, last);
			start = cur;
			last = cur;
		}
	}

	if (start != -1)
	{
		InsertEraseCmd(optimized_erase_cmds, start, last);
	}

	return optimized_erase_cmds;
}

std::vector<iCmd*> CmdBuffer::OptimizeEraseCmds(const std::vector<iCmd*>& cmds)
{
	std::set<int> erase_addrs;

	for (int i = 0; i < cmds.size(); ++i)
	{
		if (cmds[i]->GetMode() != "E")
		{
			continue;
		}

		AddEraseArea(erase_addrs, cmds[i]);
	}

	return GetMergedEraseCmds(erase_addrs);
}

std::vector<iCmd*> CmdBuffer::OptimizeCmds(std::vector<iCmd*>& cmds)
{
	std::vector<iCmd*> ret;
	std::vector<iCmd*> optimized_wrtie_cmds = OptimizeWriteCmds(cmds);
	std::vector<iCmd*> optimized_erase_cmds = OptimizeEraseCmds(cmds);

	ret = optimized_erase_cmds;
	ret.insert(ret.end(), optimized_wrtie_cmds.begin(), optimized_wrtie_cmds.end());

	return ret;
}

std::vector<iCmd*> CmdBuffer::GetOptimizedCmds()
{
	std::vector<iCmd*> cmds;
	GetiCmdList(cmds);

	std::vector<iCmd*> ret = OptimizeCmds(cmds);

	return ret;
}
