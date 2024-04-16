#include "pch.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../ssd_app_test/TestShell.cpp"

using namespace std;

class MockSSDApp : public ISSDApp
{
public:
	MOCK_METHOD(void, Write, (int addr, int data), (override));
	MOCK_METHOD(void, Read, (int addr), (override));
};

TEST(TestCaseName, TestName) 
{
	MockSSDApp app;
	TestShell test_shell;

	test_shell.set_ssd_app(&app);

	EXPECT_EQ(test_shell.get_ssd_app(), &app);
}
