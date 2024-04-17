#pragma once
#include <string>
#include <fstream>

class FileManager
{
public:
	FileManager(std::string file_name) :
		file_name_(file_name) {}

	void ReadBinaryFile(uint32_t* data, size_t size, std::streampos start = 0);
	void WriteBinaryFile(uint32_t* data, size_t size, std::streampos start = 0);
	void WriteTextFile(std::string data);

private:
	void CreateEmptyFile();
	std::string file_name_;
};
