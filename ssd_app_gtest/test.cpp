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

TEST(SSDTest, DISABLED_ReadDefaultValue)
{
	SSD ssd;
	EXPECT_TRUE(true);
}

TEST(SSDTest, DISABLED_ReadWrittenValue)
{
	SSD ssd;
	EXPECT_TRUE(true);
}