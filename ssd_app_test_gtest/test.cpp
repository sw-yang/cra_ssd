#include "pch.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../ssd_app_test/TestShell.cpp"
#include "../ssd_app_test/SSD_Adaptor.cpp"
#include "../ssd_app_test/Converter.cpp"
#include "../ssd_app_test/InvalidChecker.cpp"
#include "../ssd_app_test/Logger.cpp"

#include <iostream>
#include <fstream>
#include <string>
using namespace testing;
using namespace std;

class MockSSDApp : public ISSDApp
{
public:
	MOCK_METHOD(uint32_t, Read, (uint32_t addr), (override));
	MOCK_METHOD(void, Write, (uint32_t addr, uint32_t data), (override));
	MOCK_METHOD(void, Erase, (uint32_t addr, uint32_t size), (override));
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
		in_file = freopen(user_input_script.c_str(), "rt", stdin);

		Logger::GetLogger().SetPrintLevel(INFO);
	}
	void TearDown(void) 
	{
		out_file.close();
		fclose(in_file);
	}
	string user_input_script = "./test_script.txt";
private:
	ofstream out_file;
	FILE* in_file;
};

TEST_F(TestShellTestFixture, RunnerTest)
{
	string sample_script_path = "SampleScript.lst";
	ofstream script_fout(sample_script_path, ios_base::out | ios_base::trunc);

	string write_cmd = "Write 1 0x1234AAAA\n";
	string read_cmd = "Read 1\n";
	string full_write_cmd = "FullWrite 0x43215678\n";
	string full_read_cmd = "FullRead\n";
	string exit_cmd = "Exit\n";

	script_fout.write(write_cmd.c_str(), write_cmd.length());
	script_fout.write(read_cmd.c_str(), read_cmd.length());
	script_fout.write(full_write_cmd.c_str(), full_write_cmd.length());
	script_fout.write(full_read_cmd.c_str(), full_read_cmd.length());
	script_fout.write(exit_cmd.c_str(), exit_cmd.length());
	script_fout.close();

	string test_result_path = "./test_result.txt";
	ofstream result_out_file;
	result_out_file.open(test_result_path, ofstream::trunc | ofstream::out);
	cout.rdbuf(result_out_file.rdbuf());

	SSD_Adaptor app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);
	test_shell.ScriptRun(sample_script_path.c_str());

	auto file = freopen(test_result_path.c_str(), "rt", stdin);
	string result;
	getline(cin, result);
	EXPECT_EQ(result, "Write 1 0x1234AAAA  ---  Run...Pass");
	getline(cin, result);
	EXPECT_EQ(result, "Read 1  ---  Run...0x1234AAAA");
	getline(cin, result);
	EXPECT_EQ(result, "Pass");
	getline(cin, result);
	EXPECT_EQ(result, "FullWrite 0x43215678  ---  Run...Pass");
	getline(cin, result);
	EXPECT_EQ(result, "FullRead  ---  Run...0x43215678");
	for (int loop = 0; loop < 99; ++loop)
	{
		getline(cin, result);
		EXPECT_EQ(result, "0x43215678");
	}
	getline(cin, result);
	EXPECT_EQ(result, "Pass");

	fclose(file);
}

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

TEST_F(TestShellTestFixture, EraseTest)
{
	string user_input = "Erase 0 1";
	string exit_input = "Exit";
	cout << user_input << endl;
	cout << exit_input << endl;

	MockSSDApp app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	ISSDApp* Issd_app = test_shell.get_ssd_app();

	EXPECT_EQ(Issd_app, &app);
	EXPECT_CALL(app, Erase(_, _)).Times(1);
	test_shell.Run();
}

TEST_F(TestShellTestFixture, EraseRangeTest)
{
	string user_input = "EraseRange 0 9";
	string exit_input = "Exit";
	cout << user_input << endl;
	cout << exit_input << endl;

	MockSSDApp app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	ISSDApp* Issd_app = test_shell.get_ssd_app();

	EXPECT_EQ(Issd_app, &app);
	EXPECT_CALL(app, Erase(_, _)).Times(10);
	test_shell.Run();
}

TEST_F(TestShellTestFixture, HelpTest)
{
	string help_input = "Help";
	string exit_input = "Exit";
	cout << help_input << endl;
	cout << exit_input << endl;

	string test_result_path = "./test_result.txt";
	ofstream result_out_file;
	result_out_file.open(test_result_path, ofstream::trunc | ofstream::out);
	cout.rdbuf(result_out_file.rdbuf());

	MockSSDApp app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	test_shell.Run();

	auto file = freopen(test_result_path.c_str(), "rt", stdin);
	string result;
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
	fclose(file);
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

	auto file = freopen(test_result_path.c_str(), "rt", stdin);

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
	fclose(file);
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

	auto file = freopen(test_result_path.c_str(), "rt", stdin);

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
	fclose(file);
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

	auto file = freopen(test_result_path.c_str(), "rt", stdin);

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
	fclose(file);
}

TEST_F(TestShellTestFixture, TestApp1TestWithMock)
{
	string user_input = "testapp1";
	string exit_input = "Exit";
	cout << user_input << endl;
	cout << exit_input << endl;

	MockSSDApp app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);
	uint32_t pattern1 = 0xABCDFFFF;

	for (int addr = 0; addr < 100; ++addr)
	{
		EXPECT_CALL(app, Write(addr, pattern1));
	}
	for (int addr = 0; addr < 100; ++addr)
	{
		EXPECT_CALL(app, Read(addr));
	}

	test_shell.Run();
}

TEST_F(TestShellTestFixture, TestApp2TestWithMock)
{
	string user_input = "testapp2";
	string exit_input = "Exit";
	cout << user_input << endl;
	cout << exit_input << endl;

	MockSSDApp app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	uint32_t pattern1 = 0xAAAABBBB;
	uint32_t pattern2 = 0x12345678;

	for (uint32_t addr = 0; addr < 6; ++addr)
	{
		EXPECT_CALL(app, Write(addr, pattern1)).Times(50);
	}
	for (uint32_t addr = 0; addr < 6; ++addr)
	{
		EXPECT_CALL(app, Write(addr, pattern2));
	}
	for (uint32_t addr = 0; addr < 6; ++addr)
	{
		EXPECT_CALL(app, Read(addr));
	}

	test_shell.Run();
}

TEST_F(TestShellTestFixture, TestApp1TestWithSSD)
{
	string user_input = "testapp1";
	string exit_input = "Exit";
	cout << user_input << endl;
	cout << exit_input << endl;

	string test_result_path = "./test_result.txt";
	ofstream result_out_file;
	result_out_file.open(test_result_path, ofstream::trunc | ofstream::out);
	cout.rdbuf(result_out_file.rdbuf());

	SSD_Adaptor app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	test_shell.Run();

	auto file = freopen(test_result_path.c_str(), "rt", stdin);

	string result;
	string expected_data = "testapp1 pass";
	getline(cin, result);
	EXPECT_EQ(result, expected_data);
	fclose(file);
}

TEST_F(TestShellTestFixture, TestApp2TestWithSSD)
{
	string user_input = "testapp2";
	string exit_input = "Exit";
	cout << user_input << endl;
	cout << exit_input << endl;

	string test_result_path = "./test_result.txt";
	ofstream result_out_file;
	result_out_file.open(test_result_path, ofstream::trunc | ofstream::out);
	cout.rdbuf(result_out_file.rdbuf());

	SSD_Adaptor app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	test_shell.Run();

	auto file = freopen(test_result_path.c_str(), "rt", stdin);

	string result;
	string expected_data = "testapp2 pass";
	getline(cin, result);
	EXPECT_EQ(result, expected_data);
	fclose(file);
}

TEST_F(TestShellTestFixture, SSDWriteTest)
{
	cout << "Write 1 0x11112222" << endl;
	cout << "Exit" << endl;

	string test_result_path = "nand.txt";

	SSD_Adaptor app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	uint32_t expected_data = 0x11112222;
	int expected_addr = 1;
	uint32_t result;
	test_shell.Run();

	std::ifstream in(test_result_path, std::ios::binary);

	in.seekg(expected_addr * sizeof(uint32_t));
	in.read(reinterpret_cast<char*>(&result), sizeof(uint32_t));

	EXPECT_EQ(result, expected_data);
}

TEST_F(TestShellTestFixture, SSDReadTest)
{
	cout << "Read 1" << endl;
	cout << "Exit" << endl;

	string test_result_path = "Result.txt";

	SSD_Adaptor app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	string expected_data = "0x11112222";
	int expected_addr = 0;
	string result;
	test_shell.Run();

	std::ifstream in(test_result_path, std::ios::binary);

	in.seekg(0);
	std::getline(in, result);

	EXPECT_EQ(result, expected_data);
}
TEST_F(TestShellTestFixture, SSDEraseTest)
{
	cout << "Erase 0 1" << endl;
	cout << "Exit" << endl;

	string test_result_path = "nand.txt";

	SSD_Adaptor app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	uint32_t expected_data = 0;
	int expected_addr = 0;
	uint32_t result;
	test_shell.Run();

	std::ifstream in(test_result_path, std::ios::binary);

	in.seekg(expected_addr * sizeof(uint32_t));
	in.read(reinterpret_cast<char*>(&result), sizeof(uint32_t));

	EXPECT_EQ(result, expected_data);
}

TEST_F(TestShellTestFixture, SSDFullWriteTest)
{
	cout << "FullWrite 0x12345678" << endl;
	cout << "Exit" << endl;

	string test_result_path = "nand.txt";

	SSD_Adaptor app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	uint32_t expected_data = 0x12345678;
	uint32_t result;
	test_shell.Run();

	std::ifstream in(test_result_path, std::ios::binary);

	for (int expected_addr = 0; expected_addr < 100; expected_addr++)
	{
		in.seekg(expected_addr * sizeof(uint32_t));
		in.read(reinterpret_cast<char*>(&result), sizeof(uint32_t));

		EXPECT_EQ(result, expected_data);
	}
}

TEST_F(TestShellTestFixture, SSDFullReadTest)
{
	cout << "FullRead" << endl;
	cout << "Exit" << endl;

	string test_result_path = "Result.txt";

	SSD_Adaptor app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	string expected_data = "0x12345678";
	string result;

	test_shell.Run();
	std::ifstream in(test_result_path, std::ios::in);
	in.seekg(0);
	in >> result;

	EXPECT_EQ(result, expected_data);
}

TEST_F(TestShellTestFixture, SSDEraseRangeTest)
{
	cout << "EraseRange 0 9" << endl;
	cout << "Exit" << endl;
	const uint32_t startaddress = 0;
	const uint32_t endaddress = 9;
	string test_result_path = "nand.txt";

	SSD_Adaptor app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	uint32_t expected_data = 0;
	uint32_t result;
	test_shell.Run();

	std::ifstream in(test_result_path, std::ios::binary);

	for (int addrindex = startaddress; addrindex < endaddress; addrindex++)
	{
		in.seekg(addrindex * sizeof(uint32_t));
		in.read(reinterpret_cast<char*>(&result), sizeof(uint32_t));

		EXPECT_EQ(result, expected_data);
	}
}
