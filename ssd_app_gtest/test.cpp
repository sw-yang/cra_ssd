#include "pch.h"
#include "../ssd_app/SSD.cpp"

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

TEST_F(SSDTest, ThrowExceptionWhenInvalidAddress)
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

TEST_F(SSDTest, ReadWrittenValue)
{
	uint32_t data[100] = { 0, };
	data[0] = 0x48a7;
	WriteTestFiles(data, sizeof(data));

	ssd->Read(0);

	EXPECT_EQ("0x000048a7", ReadResultFile());
}
