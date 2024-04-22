#pragma once
#include <string>
#include <fstream>
#include <vector>

class FileManager
{
public:
	FileManager(std::string file_name) :
		file_name_(file_name) {}

	void ReadBinaryFile(uint32_t* data, size_t size, std::streampos start = 0);
	void WriteBinaryFile(uint32_t* data, size_t size, std::streampos start = 0);
	void WriteTextFile(std::string data);
	void ReadBufferFile(std::vector<std::string>& lines);
	void WriteBufferFile(std::string line);
	void ClearBufferFile();

private:
	void CreateEmptyFile();
	std::fstream OpenFile(bool is_binary = false);
	size_t GetFileSize(std::fstream& file);
	std::string file_name_;
};
