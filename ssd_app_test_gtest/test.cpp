#include "pch.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../ssd_app_test/TestShell.cpp"
class MockSSDApp {
public:
	MOCK_METHOD(void, write, (int addr, int data), ());
	MOCK_METHOD(void, read, (int addr), ());
};

TEST(TestCaseName, TestName) {
	MockSSDApp app;
	TestShell testshell;
	EXPECT_CALL(app, write(0, 0x12345678))
		.Times(1);

	app.write(0, 0x12345678);
}