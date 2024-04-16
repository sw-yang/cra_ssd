#include "pch.h"
#include "../ssd_app/SSD.cpp"

TEST(TestCaseName, DummyParsingTest) {
	SSD ssd;

	Command result = ssd.Parse("R 10 0x0000000F");
	Command dummy = { 'R', 10, 0xF };
	EXPECT_EQ(dummy, result);
}

TEST(TestCaseName, InvalidNumOfArgsTest) {
	SSD ssd;
	EXPECT_THROW(ssd.Parse("R 10"), std::invalid_argument);
}

TEST(TestCaseName, InvalidCmdTypeTest) {
	SSD ssd;
	EXPECT_THROW(ssd.Parse("A 10 0xAAAAAAAA"), std::invalid_argument);
}

TEST(TestCaseName, InvalidAddressTest) {
	SSD ssd;
	EXPECT_THROW(ssd.Parse("R 1a 0xAAAAAAAA"), std::invalid_argument);
}

TEST(TestCaseName, InvalidAddressOutOfRangeTest) {
	SSD ssd;
	EXPECT_THROW(ssd.Parse("R 100 0xAAAAAAAA"), std::invalid_argument);
}

TEST(TestCaseName, InvalidHexDataTest) {
	SSD ssd;
	EXPECT_THROW(ssd.Parse("R 10 0xAAAAAAAH"), std::invalid_argument);
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

	SSD* ssd;
	const std::string test_nand = "test_nand.txt";
	const std::string test_result = "test_result.txt";
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
	uint32_t data[100] = { 0, };
	data[0] = 0x48a7;
	WriteTestFiles(data, sizeof(data));

	ssd->Read(0);

	EXPECT_EQ("0x000048a7", ReadResultFile());
}

TEST_F(SSDTest, ReadWrittenValueFromOtherAddress)
{
	uint32_t address = 57;
	uint32_t data[100] = { 0, };
	data[address] = 0xff25abcd;
	WriteTestFiles(data, sizeof(data));

	ssd->Read(address);

	EXPECT_EQ("0xff25abcd", ReadResultFile());
}

TEST_F(SSDTest, ResetNandDataWhenFileIsInvalid)
{
	uint32_t data[50] = { 0, };	// invalid size
	data[0] = 0x48a7;
	WriteTestFiles(data, sizeof(data));

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

	uint32_t address = 33;
	uint32_t data = 0xabcd;
	ssd->Write(address, data);

	uint32_t nand_data[100];
	std::ifstream in(test_nand, std::ios::binary);
	in.read(reinterpret_cast<char*>(nand_data), sizeof(nand_data));

	EXPECT_EQ(nand_data[address], data);
}

TEST_F(SSDTest, OverWrite)
{
	uint32_t address = 57;
	uint32_t nand_data[100] = { 0, };
	nand_data[address] = 0xff25abcd;
	WriteTestFiles(nand_data, sizeof(nand_data));

	uint32_t new_data = 0x777;
	ssd->Write(address, new_data);

	std::ifstream in(test_nand, std::ios::binary);
	in.read(reinterpret_cast<char*>(nand_data), sizeof(nand_data));

	EXPECT_EQ(nand_data[address], new_data);
}