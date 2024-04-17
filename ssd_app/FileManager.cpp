#include <stdexcept>
#include "FileManager.h"

void FileManager::ReadBinaryFile(uint32_t* data, size_t size, std::streampos start)
{
	std::ifstream in(file_name_, std::ios::binary);
	if (!in.is_open())
	{
		CreateEmptyFile();
		in.open(file_name_, std::ios::binary);
	}

	in.seekg(0, std::ios::end);
	std::streampos fileSize = in.tellg();
	if (fileSize - start < size)
	{
		throw std::length_error("Invalid File Size");
	}

	in.seekg(start);
	in.read(reinterpret_cast<char*>(data), size);
}

void FileManager::WriteBinaryFile(uint32_t* data, size_t size, std::streampos start)
{
	std::fstream out(file_name_, std::ios::in | std::ios::out | std::ios::binary);
	if (!out.is_open())
	{
		CreateEmptyFile();
		out.open(file_name_, std::ios::in | std::ios::out | std::ios::binary);
	}

	out.seekp(start);
	out.write(reinterpret_cast<const char*>(data), size);
}

void FileManager::WriteTextFile(std::string data)
{
	std::ofstream out(file_name_);
	if (!out.is_open())
	{
		throw std::exception("Fail to open result file");
	}

	out << data;
}

void FileManager::CreateEmptyFile()
{
	std::ofstream out(file_name_);
	if (!out.is_open())
	{
		throw std::exception("Fail to open result file");
	}
}