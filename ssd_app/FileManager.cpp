#include <stdexcept>
#include "FileManager.h"

void FileManager::ReadBinaryFile(uint32_t* data, size_t size, std::streampos start)
{
	std::fstream file = OpenFile(true);

	size_t file_size = GetFileSize(file);
	if (file_size < start || file_size - start < size)
	{
		throw std::length_error("Invalid File Size");
	}

	file.seekg(start);
	file.read(reinterpret_cast<char*>(data), size);
}

void FileManager::WriteBinaryFile(uint32_t* data, size_t size, std::streampos start)
{
	std::fstream file = OpenFile(true);

	file.seekp(start);
	file.write(reinterpret_cast<const char*>(data), size);
}

void FileManager::WriteTextFile(std::string data)
{
	std::fstream file = OpenFile();
	file << data;
}

std::fstream FileManager::OpenFile(bool is_binary)
{
	std::fstream file;
	int openmode = std::ios::in | std::ios::out;
	if (is_binary)
	{
		openmode |= std::ios::binary;
	}

	file.open(file_name_, openmode);
	if (!file.is_open())
	{
		CreateEmptyFile();
		file.open(file_name_, openmode);
	}

	return std::move(file);
}

void FileManager::CreateEmptyFile()
{
	std::ofstream out(file_name_);
	if (!out.is_open())
	{
		throw std::exception("Fail to open result file");
	}
}

size_t FileManager::GetFileSize(std::fstream& file)
{
	file.seekg(0, std::ios::end);
	return file.tellg();
}
