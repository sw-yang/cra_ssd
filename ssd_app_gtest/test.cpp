#include "pch.h"
#include "../ssd_app/SSD.cpp"

TEST(TestCaseName, TestName) {
	SSD ssd;
	EXPECT_TRUE(true);
}

TEST(TestCaseName, DummyParsingTest) {
	SSD ssd;
	
	Command result = ssd.Parse("R 10 0x0000000F");
	Command dummy = { 'R', 10, 0xF };
	EXPECT_EQ(dummy , result);
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