#include "CmdBuffer.h"

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

void CmdBuffer::GetiCmdList(std::vector<iCmd*> cmds)
{
	std::fstream file = OpenFile();

	std::string line;
	while (std::getline(file, line))
	{

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