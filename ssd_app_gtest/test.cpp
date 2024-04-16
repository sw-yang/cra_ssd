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

	// Erase test_nand file data
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

TEST(SSDTest, DISABLED_ReadWrittenValue)
{
	SSD ssd;
	EXPECT_TRUE(true);
}