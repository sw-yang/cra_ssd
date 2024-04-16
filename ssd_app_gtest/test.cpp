#include "pch.h"
#include "../ssd_app/SSD.cpp"

TEST(SSDTest, ThrowExceptionWhenInvalidAddress)
{
	SSD ssd;
	try
	{
		ssd.Read(100);
		FAIL();
	}
	catch (std::exception& e)
	{
		EXPECT_EQ(std::string{ "Invalid address" }, std::string{ e.what() });
	}
}

TEST(SSDTest, ReadDefaultValue)
{
	const std::string test_nand = "test_nand.txt";
	const std::string test_result = "test_result.txt";

	// Erase test file data
	std::ofstream out;
	out.open(test_nand, std::ios::binary | std::ios::trunc);
	out.close();
	out.open(test_result, std::ios::trunc);
	out.close();

	// Read
	SSD ssd(test_nand, test_result);
	ssd.Read(0);

	// Check test_result
	std::ifstream in(test_result);
	std::string ret;
	if (in.is_open()) {
		in >> ret;
	}

	EXPECT_EQ("0x00000000", ret);
}

TEST(SSDTest, ReadWrittenValue)
{
	const std::string test_nand = "test_nand.txt";
	const std::string test_result = "test_result.txt";

	// Prepare test file data
	uint32_t data[100] = { 0, };
	data[0] = 0x48a7;

	std::ofstream out;
	out.open(test_nand, std::ios::binary | std::ios::trunc);
	out.write(reinterpret_cast<const char*>(data), sizeof(data));
	out.close();
	out.open(test_result, std::ios::trunc);
	out.close();

	// Read
	SSD ssd(test_nand, test_result);
	ssd.Read(0);

	// Check test_result
	std::ifstream in(test_result);
	std::string ret;
	if (in.is_open()) {
		in >> ret;
	}

	EXPECT_EQ("0x000048a7", ret);
}
