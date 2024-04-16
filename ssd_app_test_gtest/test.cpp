#include "pch.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../ssd_app_test/TestShell.cpp"

#include <iostream>
#include <fstream>
#include <string>
using namespace testing;
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

class TestShellTestFixture : public testing::Test 
{
public:
	void SetUp(void) 
	{
		//open new test script file
		out_file.open(user_input_script, ofstream::trunc | ofstream::out);
		cout.rdbuf(out_file.rdbuf());
		//reopen 
		freopen(user_input_script.c_str(), "rt", stdin);
	}
	void TearDown(void) 
	{
		out_file.close();
	}
private:
	ofstream out_file;
	string user_input_script = "./test_script.txt";
};

TEST_F(TestShellTestFixture, OutputToFile)
{
	string user_input;
	string write_input = "Write 1 0x12345678";
	string read_input = "Read 1";

	cout << write_input << endl;
	cout << read_input << endl;

	getline(cin, user_input);
	EXPECT_EQ(user_input, write_input);
	getline(cin, user_input);
	EXPECT_EQ(user_input, read_input);
}

TEST_F(TestShellTestFixture, WriteTest)
{
	MockSSDApp app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	ISSDApp* Issd_app = test_shell.get_ssd_app();

	EXPECT_EQ(Issd_app, &app);
	EXPECT_CALL(app, Write(_, _)).Times(1);
	test_shell.Run();
}