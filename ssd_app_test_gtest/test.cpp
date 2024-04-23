#include "pch.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../ssd_app_test/TestShell.cpp"
#include "../ssd_app_test/SSD_Adaptor.cpp"
#include "../ssd_app_test/Converter.cpp"
#include "../ssd_app_test/InvalidChecker.cpp"
#include "../ssd_app_test/Logger.cpp"
#include "../ssd_app_test/writer.cpp"
#include "../ssd_app_test/Reader.cpp"
#include "../ssd_app_test/Eraser.cpp"
#include "../ssd_app_test/Exiter.cpp"
#include "../ssd_app_test/Flusher.cpp"
#include "../ssd_app_test/Helper.cpp"
#include "../ssd_app_test/TestApp1.cpp"
#include "../ssd_app_test/TestApp2.cpp"

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
	MOCK_METHOD(void, Flush, (), (override));
};

class TestShellTestFixture : public testing::Test
{
public:
	void SetUp(void)
	{
		cout_buff = cout.rdbuf();

		user_input_out_file.open(user_input_script, ofstream::trunc | ofstream::out);
		test_console_out_file.open(test_result_path, ofstream::trunc | ofstream::out);
		cout.rdbuf(test_console_out_file.rdbuf());
		result_input_file.open(test_result_path, ifstream::in);

		in_file = freopen(user_input_script.c_str(), "rt", stdin);

		Logger::GetLogger().SetPrintLevel(INFO);
	}
	void TearDown(void)
	{
		fclose(in_file);

		result_input_file.close();
		test_console_out_file.close();
		user_input_out_file.close();

		cout.rdbuf(cout_buff);
	}
	ofstream user_input_out_file;
	ofstream test_console_out_file;
	ifstream result_input_file;
private:
	string test_result_path = "./test_result.txt";
	string user_input_script = "./test_script.txt";
	streambuf* cout_buff;
	FILE* in_file;
};

TEST_F(TestShellTestFixture, OutputToFile)
{
	string write_input = "Write 1 0x12345678";
	string read_input = "Read 1";

	user_input_out_file << write_input << endl;
	user_input_out_file << read_input << endl;

	string user_input;
	getline(cin, user_input);
	EXPECT_EQ(user_input, write_input);
	getline(cin, user_input);
	EXPECT_EQ(user_input, read_input);
}

TEST_F(TestShellTestFixture, WriteTest)
{
	user_input_out_file << "Write 1 0x12345678" << endl;
	user_input_out_file << "Exit" << endl;

	MockSSDApp app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	EXPECT_CALL(app, Write(_, _)).Times(1);

	test_shell.Run();
}

TEST_F(TestShellTestFixture, FullWriteTest)
{
	user_input_out_file << "FullWrite 0x12345678" << endl;
	user_input_out_file << "Exit" << endl;

	MockSSDApp app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	EXPECT_CALL(app, Write(_, _)).Times(100);

	test_shell.Run();
}

TEST_F(TestShellTestFixture, ReadTest)
{
	user_input_out_file << "Read 1" << endl;
	user_input_out_file << "Exit" << endl;

	MockSSDApp app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	EXPECT_CALL(app, Read(_)).Times(1);

	test_shell.Run();
}

TEST_F(TestShellTestFixture, FullReadTest)
{
	user_input_out_file << "FullRead" << endl;
	user_input_out_file << "Exit" << endl;

	MockSSDApp app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	EXPECT_CALL(app, Read(_)).Times(100);

	test_shell.Run();
}

TEST_F(TestShellTestFixture, EraseTest)
{
	user_input_out_file << "Erase 0 10" << endl;
	user_input_out_file << "Exit" << endl;

	MockSSDApp app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	EXPECT_CALL(app, Erase(_, _)).Times(1);

	test_shell.Run();
}

TEST_F(TestShellTestFixture, EraseRangeTest)
{
	user_input_out_file << "EraseRange 0 9" << endl;
	user_input_out_file << "Exit" << endl;

	MockSSDApp app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	EXPECT_CALL(app, Erase(_, _)).Times(10);

	test_shell.Run();
}

TEST_F(TestShellTestFixture, FlushTest)
{
	user_input_out_file << "Flush" << endl;
	user_input_out_file << "Exit" << endl;

	MockSSDApp app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	EXPECT_CALL(app, Flush()).Times(1);

	test_shell.Run();
}

TEST_F(TestShellTestFixture, HelpTest)
{
	user_input_out_file << "Help" << endl;
	user_input_out_file << "Exit" << endl;

	MockSSDApp app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	test_shell.Run();

	string result;
	getline(result_input_file, result);
	EXPECT_EQ(result, "Available commands:");
	getline(result_input_file, result);
	EXPECT_EQ(result, "Write <addr> <data>: Write data to address");
	getline(result_input_file, result);
	EXPECT_EQ(result, "Read <addr>: Read data from address");
	getline(result_input_file, result);
	EXPECT_EQ(result, "FullWrite <data>: Write data to full address");
	getline(result_input_file, result);
	EXPECT_EQ(result, "FullRead : Read data from full address");
	getline(result_input_file, result);
	EXPECT_EQ(result, "Help: Show available commands");
	getline(result_input_file, result);
	EXPECT_EQ(result, "Exit: Exit the program");
}

TEST_F(TestShellTestFixture, ExitTest)
{
	user_input_out_file << "Exit" << endl;

	MockSSDApp app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	test_shell.Run();
}

TEST_F(TestShellTestFixture, InputInvalidWrite)
{
	user_input_out_file << "Write -1 0x11112222" << endl;
	user_input_out_file << "Write 100 0x111AB222" << endl;
	user_input_out_file << "Write 10 0x111TB222" << endl;
	user_input_out_file << "Write a 0x1111B222" << endl;
	user_input_out_file << "Write $s 0x111!B222" << endl;
	user_input_out_file << "Exit" << endl;

	MockSSDApp app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	test_shell.Run();

	string expected_invalid_addr = "[Error] Invalid Address";
	string expected_invalid_data = "[Error] Invalid Data";
	string result;
	getline(result_input_file, result);
	EXPECT_EQ(result, expected_invalid_addr);
	getline(result_input_file, result);
	EXPECT_EQ(result, expected_invalid_addr);
	getline(result_input_file, result);
	EXPECT_EQ(result, expected_invalid_data);
	getline(result_input_file, result);
	EXPECT_EQ(result, expected_invalid_addr);
	getline(result_input_file, result);
	EXPECT_EQ(result, expected_invalid_addr);
}

TEST_F(TestShellTestFixture, InputInvalidRead)
{
	user_input_out_file << "Read -1" << endl;
	user_input_out_file << "Read 100" << endl;
	user_input_out_file << "Read aaa" << endl;
	user_input_out_file << "Read $das " << endl;
	user_input_out_file << "Read $s " << endl;
	user_input_out_file << "Exit" << endl;

	MockSSDApp app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	test_shell.Run();

	string expected_invalid_addr = "[Error] Invalid Address";
	string result;

	getline(result_input_file, result);
	EXPECT_EQ(result, expected_invalid_addr);
	getline(result_input_file, result);
	EXPECT_EQ(result, expected_invalid_addr);
	getline(result_input_file, result);
	EXPECT_EQ(result, expected_invalid_addr);
	getline(result_input_file, result);
	EXPECT_EQ(result, expected_invalid_addr);
	getline(result_input_file, result);
	EXPECT_EQ(result, expected_invalid_addr);
}

TEST_F(TestShellTestFixture, InputInvalidCMD)
{
	user_input_out_file << "Writa -1 0x11112222" << endl;
	user_input_out_file << "Exit" << endl;

	MockSSDApp app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	test_shell.Run();

	string expected_invalid_cmd = "[Error] Invalid CMD";
	string result;
	getline(result_input_file, result);
	EXPECT_EQ(result, expected_invalid_cmd);
	getline(result_input_file, result);
	EXPECT_EQ(result, "Available commands:");
	getline(result_input_file, result);
	EXPECT_EQ(result, "Write <addr> <data>: Write data to address");
	getline(result_input_file, result);
	EXPECT_EQ(result, "Read <addr>: Read data from address");
	getline(result_input_file, result);
	EXPECT_EQ(result, "FullWrite <data>: Write data to full address");
	getline(result_input_file, result);
	EXPECT_EQ(result, "FullRead : Read data from full address");
	getline(result_input_file, result);
	EXPECT_EQ(result, "Help: Show available commands");
	getline(result_input_file, result);
	EXPECT_EQ(result, "Exit: Exit the program");
}

TEST_F(TestShellTestFixture, TestApp1TestWithMock)
{
	user_input_out_file << "testapp1" << endl;
	user_input_out_file << "Exit" << endl;

	MockSSDApp app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	uint32_t pattern1 = 0xABCDFFFF;
	for (int addr = 0; addr < 100; ++addr)
		EXPECT_CALL(app, Write(addr, pattern1));

	for (int addr = 0; addr < 100; ++addr)
		EXPECT_CALL(app, Read(addr));

	test_shell.Run();
}

TEST_F(TestShellTestFixture, TestApp2TestWithMock)
{
	user_input_out_file << "testapp2" << endl;
	user_input_out_file << "Exit" << endl;

	MockSSDApp app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	uint32_t pattern1 = 0xAAAABBBB;
	uint32_t pattern2 = 0x12345678;

	for (uint32_t addr = 0; addr < 6; ++addr)
		EXPECT_CALL(app, Write(addr, pattern1)).Times(50);
	for (uint32_t addr = 0; addr < 6; ++addr)
		EXPECT_CALL(app, Write(addr, pattern2));
	for (uint32_t addr = 0; addr < 6; ++addr)
		EXPECT_CALL(app, Read(addr));

	test_shell.Run();
}

TEST_F(TestShellTestFixture, TestApp1TestWithSSD)
{
	user_input_out_file << "testapp1" << endl;
	user_input_out_file << "Exit" << endl;

	SSD_Adaptor app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);
	LOGGER.SetPrintLevel(ONLY_RUNNER);

	test_shell.Run();

	string result;
	string expected = "testapp1  ---  Run...Pass";

	getline(result_input_file, result);
	EXPECT_EQ(result, expected);
}

TEST_F(TestShellTestFixture, TestApp2TestWithSSD)
{
	user_input_out_file << "testapp2" << endl;
	user_input_out_file << "Exit" << endl;

	SSD_Adaptor app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);
	LOGGER.SetPrintLevel(ONLY_RUNNER);

	test_shell.Run();

	string result;
	string expected = "testapp2  ---  Run...Pass";

	getline(result_input_file, result);
	EXPECT_EQ(result, expected);
}

TEST_F(TestShellTestFixture, SSDWriteTest)
{
	user_input_out_file << "Write 1 0x11112222" << endl;
	user_input_out_file << "Flush" << endl;
	user_input_out_file << "Exit" << endl;

	SSD_Adaptor app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	test_shell.Run();

	uint32_t expected_data = 0x11112222;
	uint32_t expected_addr = 1;
	uint32_t result;

	ifstream in("nand.txt", std::ios::binary);
	in.seekg(expected_addr * sizeof(uint32_t));
	in.read(reinterpret_cast<char*>(&result), sizeof(uint32_t));
	in.close();

	EXPECT_EQ(result, expected_data);
}

TEST_F(TestShellTestFixture, SSDReadTest)
{
	user_input_out_file << "Write 1 0x11112222" << endl;
	user_input_out_file << "Read 1" << endl;
	user_input_out_file << "Flush" << endl;
	user_input_out_file << "Exit" << endl;

	SSD_Adaptor app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	test_shell.Run();

	string expected_data = "0x11112222";
	uint32_t expected_addr = 0;
	string result;

	ifstream in("result.txt", std::ios::binary);
	in.seekg(0);
	getline(in, result);
	in.close();

	EXPECT_EQ(result, expected_data);
}
TEST_F(TestShellTestFixture, SSDEraseTest)
{
	user_input_out_file << "Write 0 0x11112222" << endl;
	user_input_out_file << "Erase 0 1" << endl;
	user_input_out_file << "Flush" << endl;
	user_input_out_file << "Exit" << endl;

	SSD_Adaptor app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	test_shell.Run();

	uint32_t expected_data = 0;
	uint32_t expected_addr = 0;
	uint32_t result;

	ifstream in("nand.txt", std::ios::binary);
	in.seekg(expected_addr * sizeof(uint32_t));
	in.read(reinterpret_cast<char*>(&result), sizeof(uint32_t));
	in.close();

	EXPECT_EQ(result, expected_data);
}

TEST_F(TestShellTestFixture, SSDFullWriteTest)
{
	user_input_out_file << "FullWrite 0x12345678" << endl;
	user_input_out_file << "Flush" << endl;
	user_input_out_file << "Exit" << endl;

	SSD_Adaptor app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	test_shell.Run();

	uint32_t expected_data = 0x12345678;
	uint32_t result;

	ifstream in("nand.txt", std::ios::binary);
	for (uint32_t expected_addr = 0; expected_addr < 100; expected_addr++)
	{
		in.seekg(expected_addr * sizeof(uint32_t));
		in.read(reinterpret_cast<char*>(&result), sizeof(uint32_t));

		EXPECT_EQ(result, expected_data);
	}
	in.close();
}

TEST_F(TestShellTestFixture, SSDFullReadTest)
{
	user_input_out_file << "FullWrite 0x12345678" << endl;
	user_input_out_file << "FullRead" << endl;
	user_input_out_file << "Flush" << endl;
	user_input_out_file << "Exit" << endl;

	SSD_Adaptor app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	test_shell.Run();

	ifstream in("result.txt", std::ios::in);
	string expected_data = "0x12345678";
	string result;
	in >> result;
	in.close();

	EXPECT_EQ(result, expected_data);
}

TEST_F(TestShellTestFixture, SSDEraseRangeTest)
{
	user_input_out_file << "FullWrite 0x12345678" << endl;
	user_input_out_file << "EraseRange 0 9" << endl;
	user_input_out_file << "Flush" << endl;
	user_input_out_file << "Exit" << endl;

	SSD_Adaptor app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	test_shell.Run();

	const uint32_t startaddress = 0;
	const uint32_t endaddress = 9;
	uint32_t expected_data = 0;
	uint32_t result;

	ifstream in("nand.txt", std::ios::binary);
	for (int addrindex = startaddress; addrindex < endaddress; addrindex++)
	{
		in.seekg(addrindex * sizeof(uint32_t));
		in.read(reinterpret_cast<char*>(&result), sizeof(uint32_t));

		EXPECT_EQ(result, expected_data);
	}
	in.close();
}

TEST_F(TestShellTestFixture, SSDWriteTestWithoutFlush)
{
	user_input_out_file << "Erase 1 1" << endl; // PreCondition ExpectData : 0
	user_input_out_file << "Flush" << endl;
	user_input_out_file << "Write 1 0x11112222" << endl;
	user_input_out_file << "Exit" << endl;

	SSD_Adaptor app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	test_shell.Run();

	uint32_t expected_data = 0;
	uint32_t expected_addr = 1;
	uint32_t result;

	ifstream in("nand.txt", std::ios::binary);
	in.seekg(expected_addr * sizeof(uint32_t));
	in.read(reinterpret_cast<char*>(&result), sizeof(uint32_t));
	in.close();

	EXPECT_EQ(result, expected_data);
}

TEST_F(TestShellTestFixture, SSDReadTestWithoutFlush)
{
	user_input_out_file << "Write 1 0x11112222" << endl;
	user_input_out_file << "Read 1" << endl;
	user_input_out_file << "Exit" << endl;

	SSD_Adaptor app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	test_shell.Run();

	string expected_data = "0x11112222";
	uint32_t expected_addr = 0;
	string result;

	ifstream in("result.txt", std::ios::binary);
	in.seekg(0);
	getline(in, result);
	in.close();

	EXPECT_EQ(result, expected_data);
}
TEST_F(TestShellTestFixture, SSDEraseTestWithoutFlush)
{
	user_input_out_file << "Write 1 0x11112222" << endl;
	user_input_out_file << "Flush" << endl; // PreCondition Write
	user_input_out_file << "Erase 1 1" << endl;
	user_input_out_file << "Exit" << endl;

	SSD_Adaptor app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	test_shell.Run();

	uint32_t expected_data = 0x11112222;
	uint32_t expected_addr = 1;
	uint32_t result;

	ifstream in("nand.txt", std::ios::binary);
	in.seekg(expected_addr * sizeof(uint32_t));
	in.read(reinterpret_cast<char*>(&result), sizeof(uint32_t));
	in.close();

	EXPECT_EQ(result, expected_data);
}

TEST_F(TestShellTestFixture, SSDReadTestAfterEraseWithoutFlush)
{
	user_input_out_file << "Write 1 0x11112222" << endl;
	user_input_out_file << "Flush" << endl; // PreCondition Write
	user_input_out_file << "Erase 1 1" << endl;
	user_input_out_file << "Read 1" << endl;
	user_input_out_file << "Exit" << endl;

	SSD_Adaptor app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	test_shell.Run();

	const string expected_data = "0x00000000"; // 빈칸 데이터 0
	uint32_t expected_addr = 1;
	string result;

	ifstream in("result.txt", std::ios::in);
	in >> result;
	in.close();

	EXPECT_EQ(result, expected_data);
}

TEST_F(TestShellTestFixture, SSDFullWriteTestWithoutFlush)
{
	user_input_out_file << "FullWrite 0x12345678" << endl;
	user_input_out_file << "Exit" << endl;

	SSD_Adaptor app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	test_shell.Run();

	uint32_t expected_data = 0x12345678;
	uint32_t result;

	ifstream in("nand.txt", std::ios::binary);
	for (uint32_t expected_addr = 0; expected_addr < 100; expected_addr++)
	{
		in.seekg(expected_addr * sizeof(uint32_t));
		in.read(reinterpret_cast<char*>(&result), sizeof(uint32_t));

		EXPECT_EQ(result, expected_data);
	}
	in.close();
}

TEST_F(TestShellTestFixture, SSDFullReadTestWithoutFlush)
{
	user_input_out_file << "FullWrite 0x12345678" << endl;
	user_input_out_file << "FullRead" << endl;
	user_input_out_file << "Exit" << endl;

	SSD_Adaptor app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	test_shell.Run();

	string expected_data = "0x12345678";
	string result;

	ifstream in("result.txt", std::ios::in);
	in >> result;
	in.close();

	EXPECT_EQ(result, expected_data);
}

TEST_F(TestShellTestFixture, SSDEraseRangeTestWithoutFlush)
{
	user_input_out_file << "FullWrite 0x12345678" << endl;
	user_input_out_file << "EraseRange 0 9" << endl;
	user_input_out_file << "Exit" << endl;

	SSD_Adaptor app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);

	test_shell.Run();

	const uint32_t startaddress = 0;
	const uint32_t endaddress = 9;
	uint32_t expected_data = 0;
	uint32_t result;

	ifstream in("nand.txt", std::ios::binary);
	for (int addrindex = startaddress; addrindex < endaddress; addrindex++)
	{
		in.seekg(addrindex * sizeof(uint32_t));
		in.read(reinterpret_cast<char*>(&result), sizeof(uint32_t));

		EXPECT_EQ(result, expected_data);
	}
	in.close();
}

TEST_F(TestShellTestFixture, RunnerTest)
{
	string sample_script_path = "SampleScript.lst";
	string write_cmd = "Write 1 0x1234AAAA";
	string read_cmd = "Read 1";
	string full_write_cmd = "FullWrite 0x43215678";
	string full_read_cmd = "FullRead";
	string exit_cmd = "Exit";

	ofstream script_fout(sample_script_path, ios_base::out | ios_base::trunc);
	script_fout << write_cmd << endl;
	script_fout << read_cmd << endl;
	script_fout << full_write_cmd << endl;
	script_fout << full_read_cmd << endl;
	script_fout << exit_cmd << endl;
	script_fout.close();

	SSD_Adaptor app;
	TestShell test_shell;
	test_shell.set_ssd_app(&app);
	test_shell.ScriptRun(sample_script_path.c_str());

	string result;
	getline(result_input_file, result);
	EXPECT_EQ(result, write_cmd + "  ---  Run...Pass");
	getline(result_input_file, result);
	EXPECT_EQ(result, read_cmd + "  ---  Run...Pass");
	getline(result_input_file, result);
	EXPECT_EQ(result, full_write_cmd + "  ---  Run...Pass");
	getline(result_input_file, result);
	EXPECT_EQ(result, full_read_cmd + "  ---  Run...Pass");
}