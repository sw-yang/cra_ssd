#include "pch.h"
#include "../ssd_app/SSD.cpp"
#include "../ssd_app/CMD.cpp"

class CMDTestFixture : public testing::Test 
{
protected:
	vector<string> args;
};

TEST_F(CMDTestFixture, ValidObjectVectorConstructorTest) 
{
	args.push_back("R");
	args.push_back("10");
	args.push_back("0xAAAAAAAA");

	Command cmd(args);
	Command result('R', 10, 0xAAAAAAAA);
	
	EXPECT_EQ(cmd, result);
}

TEST_F(CMDTestFixture, ValidObjectStringConstructorTest)
{
	Command cmd("R 10 0xAAAAAAAA");
	Command result('R', 10, 0xAAAAAAAA);

	EXPECT_EQ(cmd, result);
}

TEST_F(CMDTestFixture, ValidFlagTest) 
{
	args.push_back("R");
	args.push_back("10");
	args.push_back("0xAAAAAAAA");

	Command cmd(args);

	EXPECT_EQ(true, cmd.getValid());
}

TEST_F(CMDTestFixture, InvalidCommandTypeTest) 
{
	args.push_back("A");
	args.push_back("10");
	args.push_back("0xAAAAAAAA");

	Command cmd(args);

	EXPECT_EQ(false, cmd.getValid());
}

TEST_F(CMDTestFixture, AddressOutOfRangeTest) 
{
	args.push_back("R");
	args.push_back("100");
	args.push_back("0xAAAAAAAA");

	Command cmd(args);

	EXPECT_EQ(false, cmd.getValid());
}

TEST_F(CMDTestFixture, AddressTypeErrorTest) 
{
	args.push_back("R");
	args.push_back("1a");
	args.push_back("0xAAAAAAAA");

	Command cmd(args);

	EXPECT_EQ(false, cmd.getValid());
}

TEST_F(CMDTestFixture, DataLengthErrorTest) 
{
	args.push_back("R");
	args.push_back("10");
	args.push_back("0xAAAAAAA");

	Command cmd(args);

	EXPECT_EQ(false, cmd.getValid());
}

TEST_F(CMDTestFixture, DataTypeErrorTest) {
	args.push_back("R");
	args.push_back("10");
	args.push_back("0xAAAAAAAH");

	Command cmd(args);

	EXPECT_EQ(false, cmd.getValid());
}

class SSDTest : public testing::Test
{
protected:
	void SetUp() override
	{
		ssd = new SSD(test_nand, test_result);
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

	SSD* ssd;
	const std::string test_nand = "test_nand.txt";
	const std::string test_result = "test_result.txt";

	uint32_t NAND[100] = { 0, };
	uint32_t ADDRESS = 57;
};

TEST_F(SSDTest, ThrowExceptionWhenInvalidAddressWhileRead)
{
	try
	{
		ssd->Read(100);
		FAIL();
	}
	catch (std::exception& e)
	{
		EXPECT_EQ(std::string{ "Invalid address" }, std::string{ e.what() });
	}
}

TEST_F(SSDTest, ReadDefaultValue)
{
	ClearTestFiles();

	ssd->Read(0);

	EXPECT_EQ("0x00000000", ReadResultFile());
}

TEST_F(SSDTest, ReadWrittenValueFrom0)
{
	NAND[0] = 0x48a7;
	WriteTestFiles(NAND, sizeof(NAND));

	ssd->Read(0);

	EXPECT_EQ("0x000048a7", ReadResultFile());
}

TEST_F(SSDTest, ReadWrittenValueFromOtherAddress)
{
	NAND[ADDRESS] = 0xff25abcd;
	WriteTestFiles(NAND, sizeof(NAND));

	ssd->Read(ADDRESS);

	EXPECT_EQ("0xff25abcd", ReadResultFile());
}

TEST_F(SSDTest, ResetNandDataWhenFileIsInvalid)
{
	uint32_t INVALID_NAND[50] = { 0, };	// invalid size
	INVALID_NAND[0] = 0x48a7;
	WriteTestFiles(INVALID_NAND, sizeof(INVALID_NAND));

	ssd->Read(0);

	EXPECT_EQ("0x00000000", ReadResultFile());
}

TEST_F(SSDTest, ThrowExceptionWhenInvalidAddressWhileWrite)
{
	try
	{
		ssd->Write(100, 0);
		FAIL();
	}
	catch (std::exception& e)
	{
		EXPECT_EQ(std::string{ "Invalid address" }, std::string{ e.what() });
	}
}

TEST_F(SSDTest, WriteFirstTime)
{
	ClearTestFiles();

	uint32_t data = 0xabcd;
	ssd->Write(ADDRESS, data);

	ReadNandFile(NAND, sizeof(NAND));

	EXPECT_EQ(NAND[ADDRESS], data);
}

TEST_F(SSDTest, OverWrite)
{
	NAND[ADDRESS] = 0xff25abcd;
	WriteTestFiles(NAND, sizeof(NAND));

	uint32_t new_data = 0x777;
	ssd->Write(ADDRESS, new_data);

	ReadNandFile(NAND, sizeof(NAND));

	EXPECT_EQ(NAND[ADDRESS], new_data);
}

TEST_F(SSDTest, ReadWriteTest)
{
	ssd->Write(10, 0x11);
	ssd->Read(10);
	EXPECT_EQ("0x00000011", ReadResultFile());

	ssd->Write(10, 0x777);
	ssd->Read(10);
	EXPECT_EQ("0x00000777", ReadResultFile());

	ssd->Read(20);
	EXPECT_EQ("0x00000000", ReadResultFile());

	ssd->Write(20, 0x777);
	ssd->Read(20);
	EXPECT_EQ("0x00000777", ReadResultFile());
}

