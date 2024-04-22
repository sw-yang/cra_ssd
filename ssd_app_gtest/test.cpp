#include <cstdio>
#include "pch.h"
#include "../ssd_app/SSD.cpp"
#include "../ssd_app/FileManager.cpp"
#include "../ssd_app/Reader.cpp"
#include "../ssd_app/Writer.cpp"
#include "../ssd_app/Eraser.cpp"
#include "../ssd_app/CmdBuffer.cpp"

class FileManagerTest : public testing::Test
{
protected:
	void SetUp() override
	{
		std::ofstream out;
		out.open(test_file, std::ios::trunc);
		out.close();

		fm = new FileManager(test_file);
	}

public:
	void ReadBinaryFile()
	{
		std::ifstream in(test_file, std::ios::binary);
		in.read(reinterpret_cast<char*>(data), sizeof(data));
	}

	void WriteBinaryFiles()
	{
		std::ofstream out;
		out.open(test_file, std::ios::binary);
		out.write(reinterpret_cast<char*>(data), sizeof(data));
		out.close();
	}

	std::string ReadTextFile()
	{
		std::string ret;
		std::ifstream in(test_file);
		in >> ret;
		return ret;
	}

	void RemoveFile()
	{
		std::remove(test_file.c_str());
	}

	FileManager* fm;
	const std::string test_file = "test.txt";
	uint32_t data[5] = { 1, 2, 3, 4, 5 };
};

TEST_F(FileManagerTest, ThrowLengthErrorFromReadBinaryFile)
{
	WriteBinaryFiles();
	try
	{
		fm->ReadBinaryFile(data, sizeof(data) + 100);
		FAIL();
	}
	catch (std::length_error& e)
	{
		EXPECT_EQ(std::string{ "Invalid File Size" }, std::string{ e.what() });
	}
}

TEST_F(FileManagerTest, CreateNewFileInReadWhenNotExist)
{
	RemoveFile();
	try
	{
		fm->ReadBinaryFile(data, sizeof(data));
		FAIL();
	}
	catch (std::length_error& e)
	{
		EXPECT_EQ(std::string{ "Invalid File Size" }, std::string{ e.what() });
	}
}

TEST_F(FileManagerTest, ReadBinaryFileFromFirst)
{
	WriteBinaryFiles();

	uint32_t ret[5];
	fm->ReadBinaryFile(ret, sizeof(ret));

	EXPECT_TRUE(std::equal(
		std::begin(data), std::end(data), std::begin(ret)));
}

TEST_F(FileManagerTest, ReadBinaryFileFromMiddle)
{
	WriteBinaryFiles();

	uint32_t idx = 2;
	uint32_t ret;
	fm->ReadBinaryFile(&ret, sizeof(ret), idx * sizeof(ret));

	EXPECT_EQ(data[idx], ret);
}

TEST_F(FileManagerTest, CreateNewFileInWriteWhenNotExist)
{
	RemoveFile();
	uint32_t new_data[5] = { 9, 8, 7, 6, 5 };
	fm->WriteBinaryFile(new_data, sizeof(new_data));

	ReadBinaryFile();

	EXPECT_TRUE(std::equal(
		std::begin(new_data), std::end(new_data), std::begin(data)));
}


TEST_F(FileManagerTest, WriteBinaryFileFromFirst)
{
	uint32_t new_data[5] = { 9, 8, 7, 6, 5 };
	fm->WriteBinaryFile(new_data, sizeof(new_data));

	ReadBinaryFile();

	EXPECT_TRUE(std::equal(
		std::begin(new_data), std::end(new_data), std::begin(data)));
}

TEST_F(FileManagerTest, WriteBinaryFileFromMiddle)
{
	uint32_t idx = 2;
	uint32_t ret = 7;
	fm->WriteBinaryFile(&ret, sizeof(ret), idx * sizeof(ret));

	ReadBinaryFile();

	EXPECT_EQ(ret, data[idx]);
}

TEST_F(FileManagerTest, WriteTextFile)
{
	std::string text = "test";
	fm->WriteTextFile(text);

	EXPECT_EQ(ReadTextFile(), text);
}


class SSDTest : public testing::Test
{
protected:
	void SetUp() override
	{
		ssd = new SSD(test_nand, test_result, test_buffer);
	}

public:
	void WriteTestFiles(uint32_t* data, size_t size)
	{
		std::ofstream out;
		out.open(test_nand, std::ios::binary | std::ios::trunc);
		out.write(reinterpret_cast<const char*>(data), size);
		out.close();
		out.open(test_result, std::ios::trunc);
		out.close();
	}

	void ClearTestFiles()
	{
		std::ofstream out;
		out.open(test_nand, std::ios::binary | std::ios::trunc);
		out.close();
		out.open(test_result, std::ios::trunc);
		out.close();
	}

	std::string ReadResultFile()
	{
		std::ifstream in(test_result);
		std::string ret;
		if (in.is_open()) {
			in >> ret;
		}
		return ret;
	}

	void ReadNandFile(uint32_t* data, size_t size)
	{
		std::ifstream in(test_nand, std::ios::binary);
		in.read(reinterpret_cast<char*>(data), size);
	}

	void ReadBufferFile(std::vector<std::string>& cmds)
	{
		std::ifstream in(test_buffer);

		std::string line;

		while (std::getline(in, line)) {
			cmds.push_back(line);
		}

		in.close();
	}

	SSD* ssd;
	const std::string test_nand = "test_nand.txt";
	const std::string test_result = "test_result.txt";
	const std::string test_buffer = "test_buffer.txt";

	uint32_t NAND[100] = { 0, };
	uint32_t ADDRESS = 57;
};

TEST_F(SSDTest, ThrowExceptionWhenInvalidMode)
{
	try
	{
		std::vector<std::string> args = { "100" };
		ssd->Run("A", args);
		FAIL();
	}
	catch (std::exception& e)
	{
		EXPECT_EQ(std::string{ "Invalid mode" }, std::string{ e.what() });
	}
}

TEST_F(SSDTest, ThrowExceptionWhenInvalidArgsNumWhileRead)
{
	try
	{
		std::vector<std::string> args = { "0", "aaa" };
		ssd->Run("R", args);
		FAIL();
	}
	catch (std::exception& e)
	{
		EXPECT_EQ(std::string{ "Invalid args" }, std::string{ e.what() });
	}
}

TEST_F(SSDTest, ThrowExceptionWhenInvalidHexWhileRead)
{
	try
	{
		std::vector<std::string> args = { "0", "0x111111GG" };
		ssd->Run("R", args);
		FAIL();
	}
	catch (std::exception& e)
	{
		EXPECT_EQ(std::string{ "Invalid args" }, std::string{ e.what() });
	}
}

TEST_F(SSDTest, ThrowExceptionWhenInvalidAddressWhileRead)
{
	try
	{
		std::vector<std::string> args = { "100" };
		ssd->Run("R", args);
		FAIL();
	}
	catch (std::exception& e)
	{
		EXPECT_EQ(std::string{ "Invalid args" }, std::string{ e.what() });
	}
}

TEST_F(SSDTest, ReadDefaultValue)
{
	ClearTestFiles();

	std::vector<std::string> args = { "0" };
	ssd->Run("R", args);

	EXPECT_EQ("0x00000000", ReadResultFile());
}

TEST_F(SSDTest, ReadWrittenValueFrom0)
{
	NAND[0] = 0x48a7;
	WriteTestFiles(NAND, sizeof(NAND));

	std::vector<std::string> args = { "0" };
	ssd->Run("R", args);

	EXPECT_EQ("0x000048A7", ReadResultFile());
}

TEST_F(SSDTest, ReadWrittenValueFromOtherAddress)
{
	NAND[ADDRESS] = 0xff25abcd;
	WriteTestFiles(NAND, sizeof(NAND));

	std::vector<std::string> args;
	args.push_back(std::to_string(ADDRESS));
	ssd->Run("R", args);

	EXPECT_EQ("0xFF25ABCD", ReadResultFile());
}

TEST_F(SSDTest, ResetNandDataWhenFileIsInvalid)
{
	uint32_t INVALID_NAND[50] = { 0, };	// invalid size
	INVALID_NAND[0] = 0x48a7;
	WriteTestFiles(INVALID_NAND, sizeof(INVALID_NAND));

	std::vector<std::string> args = { "0" };
	ssd->Run("R", args);

	EXPECT_EQ("0x00000000", ReadResultFile());
}

TEST_F(SSDTest, ThrowExceptionWhenInvalidAddressWhileWrite)
{
	try
	{
		std::vector<std::string> args = { "100", "0x00000000" };
		ssd->Run("W", args);
		FAIL();
	}
	catch (std::exception& e)
	{
		EXPECT_EQ(std::string{ "Invalid args" }, std::string{ e.what() });
	}
}

TEST_F(SSDTest, DISABLED_ThrowExceptionWhenInvalidArgsNumWhileWrite)
{
	try
	{
		std::vector<std::string> args = { "1" };
		ssd->Run("W", args);
		FAIL();
	}
	catch (std::exception& e)
	{
		EXPECT_EQ(std::string{ "Invalid args" }, std::string{ e.what() });
	}
}

TEST_F(SSDTest, WriteFirstTime)
{
	ClearTestFiles();

	std::vector<std::string> args;
	args.push_back(std::to_string(ADDRESS));
	args.push_back("0x0000ABCD");
	ssd->Run("W", args);

	ReadNandFile(NAND, sizeof(NAND));

	EXPECT_EQ(NAND[ADDRESS], 0xabcd);
}

TEST_F(SSDTest, OverWrite)
{
	std::vector<std::string> args;
	args.push_back(std::to_string(ADDRESS));
	args.push_back("0xFF25ABCD");
	ssd->Run("W", args);

	args.clear();
	args.push_back(std::to_string(ADDRESS));
	args.push_back("0x00000777");
	ssd->Run("W", args);

	ReadNandFile(NAND, sizeof(NAND));

	EXPECT_EQ(NAND[ADDRESS], 0x777);
}

TEST_F(SSDTest, ReadWriteTest)
{
	ssd->Run("W", { "10", "0x00000011" });
	ssd->Run("R", { "10" });
	EXPECT_EQ("0x00000011", ReadResultFile());

	ssd->Run("W", { "10", "0x00000777" });
	ssd->Run("R", { "10" });
	EXPECT_EQ("0x00000777", ReadResultFile());

	ssd->Run("R", { "20" });
	EXPECT_EQ("0x00000000", ReadResultFile());

	ssd->Run("W", { "20", "0x00000777" });
	ssd->Run("R", { "20" });
	EXPECT_EQ("0x00000777", ReadResultFile());
}

TEST_F(SSDTest, ThrowExceptionWhenInvalidStartAddressWhileErase)
{
	try
	{
		std::vector<std::string> args = { "100", "1" };
		ssd->Run("E", args);
		FAIL();
	}
	catch (std::exception& e)
	{
		EXPECT_EQ(std::string{ "Invalid args" }, std::string{ e.what() });
	}
}

TEST_F(SSDTest, ThrowExceptionWhenInvalidArgsNumWhileErase)
{
	try
	{
		std::vector<std::string> args = { "0" };
		ssd->Run("E", args);
		FAIL();
	}
	catch (std::exception& e)
	{
		EXPECT_EQ(std::string{ "Invalid args" }, std::string{ e.what() });
	}
}

TEST_F(SSDTest, ThrowExceptionWhenRangeIsZeroWhileErase)
{
	try
	{
		std::vector<std::string> args = { "0", "0" };
		ssd->Run("E", args);
		FAIL();
	}
	catch (std::exception& e)
	{
		EXPECT_EQ(std::string{ "Invalid args" }, std::string{ e.what() });
	}
}

TEST_F(SSDTest, ThrowExceptionWhenRangeOverTenWhileErase)
{
	try
	{
		std::vector<std::string> args = { "0", "20" };
		ssd->Run("E", args);
		FAIL();
	}
	catch (std::exception& e)
	{
		EXPECT_EQ(std::string{ "Invalid args" }, std::string{ e.what() });
	}
}

TEST_F(SSDTest, ThrowExceptionWhenInvalidAddressWhileErase)
{
	try
	{
		std::vector<std::string> args = { "95", "8" };
		ssd->Run("E", args);
		FAIL();
	}
	catch (std::exception& e)
	{
		EXPECT_EQ(std::string{ "Invalid args" }, std::string{ e.what() });
	}
}

TEST_F(SSDTest, EraseData)
{
	int addr = 0;
	int range = 7;
	
	for (int i = 0; i < 100; ++i)
	{
		NAND[i] = 9999;
	}	
	WriteTestFiles(NAND, sizeof(NAND));


	std::vector<std::string> args;
	args.push_back(std::to_string(addr));
	args.push_back(std::to_string(range));
	ssd->Run("E", args);

	ReadNandFile(NAND, sizeof(NAND));
	for (int i = addr; i < range; ++i)
	{
		EXPECT_EQ(NAND[i], 0);
	}
}

TEST_F(SSDTest, EraseAfterWrite)
{
	std::vector<std::string> args;
	args.push_back(std::to_string(ADDRESS));
	args.push_back("0xFF25ABCD");
	ssd->Run("W", args);

	ReadNandFile(NAND, sizeof(NAND));

	EXPECT_EQ(NAND[ADDRESS], 0xFF25ABCD);

	args.clear();
	args.push_back(std::to_string(ADDRESS));
	args.push_back("1");
	ssd->Run("E", args);

	ReadNandFile(NAND, sizeof(NAND));

	EXPECT_EQ(NAND[ADDRESS], 0);
}

TEST_F(SSDTest, AddAndGetCommand)
{
	CmdBuffer* cmd_buffer = ssd->GetCmdBuffer();
	cmd_buffer->Clear();

	std::vector<std::string> args;

	args.push_back(std::to_string(ADDRESS));
	args.push_back("0xFF25ABCD");
	ssd->Run("W", args);

	args.clear();
	args.push_back(std::to_string(ADDRESS));
	ssd->Run("R", args);

	args.clear();
	args.push_back(std::to_string(ADDRESS));
	args.push_back("9");
	ssd->Run("E", args);

	std::vector<std::string> cmds;
	cmd_buffer->GetCmd(cmds);

	EXPECT_EQ(cmds[0], "W 57 0xFF25ABCD");
	EXPECT_EQ(cmds[1], "R 57");
	EXPECT_EQ(cmds[2], "E 57 9");
}

TEST_F(SSDTest, AddAndGetCommandList)
{
	CmdBuffer* cmd_buffer = ssd->GetCmdBuffer();
	cmd_buffer->Clear();

	std::vector<std::string> args;

	args.push_back(std::to_string(ADDRESS));
	args.push_back("0xFF25ABCD");
	ssd->Run("W", args);

	std::vector<std::vector<std::string>> cmds;
	cmd_buffer->GetCmdList(cmds);

	EXPECT_EQ(cmds[0][0], "W");
	EXPECT_EQ(cmds[0][1], "57");
	EXPECT_EQ(cmds[0][2], "0xFF25ABCD");
}

TEST_F(SSDTest, AddAndGetCommandListICmd)
{
	CmdBuffer* cmd_buffer = ssd->GetCmdBuffer();
	cmd_buffer->Clear();

	std::vector<std::string> args;

	args.push_back(std::to_string(ADDRESS));
	args.push_back("0xFFFFFFFF");
	ssd->Run("W", args);

	args.clear();
	args.push_back(std::to_string(ADDRESS));
	ssd->Run("R", args);

	args.clear();
	args.push_back(std::to_string(ADDRESS));
	args.push_back("9");
	ssd->Run("E", args);

	std::vector<iCmd*> cmds;
	cmd_buffer->GetiCmdList(cmds);

	EXPECT_EQ(cmds[0]->ToString(), "W 57 0xFFFFFFFF");
	EXPECT_EQ(cmds[1]->ToString(), "R 57");
	EXPECT_EQ(cmds[2]->ToString(), "E 57 9");
}

TEST_F(SSDTest, ComapareGetCmdWithBufferFile)
{
	CmdBuffer* cmd_buffer = ssd->GetCmdBuffer();
	cmd_buffer->Clear();

	std::vector<std::string> args;

	args.push_back(std::to_string(ADDRESS));
	args.push_back("0xFF25ABCD");
	ssd->Run("W", args);

	args.clear();
	args.push_back(std::to_string(ADDRESS));
	ssd->Run("R", args);

	args.clear();
	args.push_back(std::to_string(ADDRESS));
	args.push_back("9");
	ssd->Run("E", args);

	std::vector<std::string> getcmds;
	cmd_buffer->GetCmd(getcmds);

	std::vector<std::string> filecmds;
	ReadBufferFile(filecmds);

	for (int i = 0; i < getcmds.size(); i++)
	{
		EXPECT_EQ(getcmds[i], filecmds[i]);
	}
}

TEST_F(SSDTest, ReturnTrueWhenBufferisFull)
{
	CmdBuffer* cmd_buffer = ssd->GetCmdBuffer();
	cmd_buffer->Clear();

	std::vector<std::string> args;
	args.push_back(std::to_string(ADDRESS));

	for (int i = 0; i < 10; i++)
	{
		ssd->Run("R", args);
	}

	EXPECT_EQ(cmd_buffer->isFull(), true);
}

TEST_F(SSDTest, ReturnBufferSize)
{
	CmdBuffer* cmd_buffer = ssd->GetCmdBuffer();
	cmd_buffer->Clear();

	std::vector<std::string> args;
	args.push_back(std::to_string(ADDRESS));

	for (int i = 0; i < 5; i++)
	{
		ssd->Run("R", args);
	}

	EXPECT_EQ(cmd_buffer->GetSize(), 5);
}