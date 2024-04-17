#include "pch.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../ssd_app_test/TestShell.cpp"
#include "../ssd_app_test/SSD_Adaptor.cpp"

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
	string user_input = "Write 1 0x12345678";
	string exit_input = "Exit";
	cout << user_input << endl;
	cout << exit_input << endl;

	MockSSDApp app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	ISSDApp* Issd_app = test_shell.get_ssd_app();

	EXPECT_EQ(Issd_app, &app);
	EXPECT_CALL(app, Write(_, _)).Times(1);
	test_shell.Run();
}

TEST_F(TestShellTestFixture, FullWriteTest)
{
	string user_input = "FullWrite 0x12345678";
	string exit_input = "Exit";
	cout << user_input << endl;
	cout << exit_input << endl;

	MockSSDApp app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	ISSDApp* Issd_app = test_shell.get_ssd_app();

	EXPECT_EQ(Issd_app, &app);
	EXPECT_CALL(app, Write(_, _)).Times(100);
	test_shell.Run();
}

TEST_F(TestShellTestFixture, ReadTest)
{
	string user_input = "Read 1";
	string exit_input = "Exit";
	cout << user_input << endl;
	cout << exit_input << endl;

	MockSSDApp app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	ISSDApp* Issd_app = test_shell.get_ssd_app();

	EXPECT_EQ(Issd_app, &app);
	EXPECT_CALL(app, Read(_)).Times(1);
	test_shell.Run();
}

TEST_F(TestShellTestFixture, FullReadTest)
{
	string user_input = "FullRead";
	string exit_input = "Exit";
	cout << user_input << endl;
	cout << exit_input << endl;

	MockSSDApp app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	ISSDApp* Issd_app = test_shell.get_ssd_app();

	EXPECT_EQ(Issd_app, &app);
	EXPECT_CALL(app, Read(_)).Times(100);
	test_shell.Run();
}
TEST_F(TestShellTestFixture, HelpTest)
{
	string help_input = "Help";
	string exit_input = "Exit";
	cout << help_input << endl;
	cout << exit_input << endl;

	MockSSDApp app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	test_shell.Run();
}
TEST_F(TestShellTestFixture, ExitTest)
{
	string user_input;
	string write_input = "Exit";
	cout << write_input << endl;

	MockSSDApp app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	test_shell.Run();
}

TEST_F(TestShellTestFixture, InputNormalWrite)
{
	cout << "Write 0 0x11112222" << endl;
	cout << "Write 99 0x111AB222" << endl;
	cout << "Exit" << endl;

	MockSSDApp app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	test_shell.Run();

	EXPECT_EQ(test_shell.addr_arr[0], 0);
	EXPECT_EQ(test_shell.data_arr[0], 0x11112222);

	EXPECT_EQ(test_shell.addr_arr[1], 99);
	EXPECT_EQ(test_shell.data_arr[1], 0x111AB222);
}

TEST_F(TestShellTestFixture, InputNormalRead)
{
	cout << "Read 4" << endl;
	cout << "Read 49" << endl;
	cout << "Exit" << endl;

	MockSSDApp app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	test_shell.Run();

	EXPECT_EQ(test_shell.addr_arr[0], 4);

	EXPECT_EQ(test_shell.addr_arr[1], 49);
}

TEST_F(TestShellTestFixture, InputInvalidWrite)
{
	cout << "Write -1 0x11112222" << endl;
	cout << "Write 100 0x111AB222" << endl;
	cout << "Write 10 0x111TB222" << endl;
	cout << "Write a 0x1111B222" << endl;
	cout << "Write $s 0x111!B222" << endl;
	cout << "Exit" << endl;

	string test_result_path = "./test_result.txt";
	ofstream result_out_file;
	result_out_file.open(test_result_path, ofstream::trunc | ofstream::out);
	cout.rdbuf(result_out_file.rdbuf());

	MockSSDApp app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	string expected_invalid_addr = "[Error] Invalid Address";
	string expected_invalid_data = "[Error] Invalid Data";
	string result;
	test_shell.Run();

	freopen(test_result_path.c_str(), "rt", stdin);

	getline(cin, result);
	EXPECT_EQ(result, expected_invalid_addr);
	getline(cin, result);
	EXPECT_EQ(result, expected_invalid_addr);
	getline(cin, result);
	EXPECT_EQ(result, expected_invalid_data);
	getline(cin, result);
	EXPECT_EQ(result, expected_invalid_addr);
	getline(cin, result);
	EXPECT_EQ(result, expected_invalid_addr);

	result_out_file.close();
}

TEST_F(TestShellTestFixture, InputInvalidRead)
{
	cout << "Read -1" << endl;
	cout << "Read 100" << endl;
	cout << "Read aaa" << endl;
	cout << "Read $das " << endl;
	cout << "Read $s " << endl;
	cout << "Exit" << endl;

	string test_result_path = "./test_result.txt";
	ofstream result_out_file;
	result_out_file.open(test_result_path, ofstream::trunc | ofstream::out);
	cout.rdbuf(result_out_file.rdbuf());

	MockSSDApp app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	string expected_invalid_addr = "[Error] Invalid Address";
	string result;
	test_shell.Run();

	freopen(test_result_path.c_str(), "rt", stdin);

	getline(cin, result);
	EXPECT_EQ(result, expected_invalid_addr);
	getline(cin, result);
	EXPECT_EQ(result, expected_invalid_addr);
	getline(cin, result);
	EXPECT_EQ(result, expected_invalid_addr);
	getline(cin, result);
	EXPECT_EQ(result, expected_invalid_addr);
	getline(cin, result);
	EXPECT_EQ(result, expected_invalid_addr);

	result_out_file.close();
}

TEST_F(TestShellTestFixture, InputInvalidCMD)
{
	cout << "Writa -1 0x11112222" << endl;
	cout << "Exit" << endl;

	string test_result_path = "./test_result.txt";
	ofstream result_out_file;
	result_out_file.open(test_result_path, ofstream::trunc | ofstream::out);
	cout.rdbuf(result_out_file.rdbuf());

	MockSSDApp app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	string expected_invalid_cmd = "[Error] Invalid CMD";
	string result;
	test_shell.Run();

	freopen(test_result_path.c_str(), "rt", stdin);

	getline(cin, result);
	EXPECT_EQ(result, expected_invalid_cmd);
	getline(cin, result);
	EXPECT_EQ(result, string{ "Available commands:" });
	getline(cin, result);
	EXPECT_EQ(result, string{ "Write <addr> <data>: Write data to address" });
	getline(cin, result);
	EXPECT_EQ(result, string{ "Read <addr>: Read data from address" });
	getline(cin, result);
	EXPECT_EQ(result, string{ "FullWrite <data>: Write data to full address" });
	getline(cin, result);
	EXPECT_EQ(result, string{ "FullRead : Read data from full address" });
	getline(cin, result);
	EXPECT_EQ(result, string{ "Help: Show available commands" });
	getline(cin, result);
	EXPECT_EQ(result, string{ "Exit: Exit the program" });
	
	result_out_file.close();
}

TEST_F(TestShellTestFixture, DISABLED_SSDWriteTest)
{
	string user_input = "Write 1 0x12345678";
	string exit_input = "Exit";
	cout << user_input << endl;
	cout << exit_input << endl;

	SSD_Adaptor app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	ISSDApp* Issd_app = test_shell.get_ssd_app();

	EXPECT_EQ(Issd_app, &app);
	test_shell.Run();
}

TEST_F(TestShellTestFixture, DISABLED_SSDFullWriteTest)
{
	string user_input = "FullWrite 0x12345678";
	string exit_input = "Exit";
	cout << user_input << endl;
	cout << exit_input << endl;

	SSD_Adaptor app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	ISSDApp* Issd_app = test_shell.get_ssd_app();

	EXPECT_EQ(Issd_app, &app);
	test_shell.Run();
}

TEST_F(TestShellTestFixture, DISABLED_SSDReadTest)
{
	string user_input = "Read 1";
	string exit_input = "Exit";
	cout << user_input << endl;
	cout << exit_input << endl;

	SSD_Adaptor app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	ISSDApp* Issd_app = test_shell.get_ssd_app();

	EXPECT_EQ(Issd_app, &app);
	test_shell.Run();
}

TEST_F(TestShellTestFixture, DISABLED_SSDFullReadTest)
{
	string user_input = "FullRead";
	string exit_input = "Exit";
	cout << user_input << endl;
	cout << exit_input << endl;

	SSD_Adaptor app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	ISSDApp* Issd_app = test_shell.get_ssd_app();

	EXPECT_EQ(Issd_app, &app);
	test_shell.Run();
}
