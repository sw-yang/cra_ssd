#define _CRT_SECURE_NO_WARNINGS

#include "TestShell.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

uint32_t
TestShell::Read(const uint32_t addr)
{
    uint32_t data = ssd_app->Read(addr);

    PrintOutALine(INFO, UintToHexString(data));

    return data;
}

vector<uint32_t>
TestShell::FullRead(void)
{
    vector<uint32_t> read_result;
    for (int addr = kMinAddr; addr <= kMaxAddr; addr++)
        read_result.push_back(Read(addr));
    
    return read_result;
}

void
TestShell::Write(const uint32_t addr, const uint32_t data)
{
    ssd_app->Write(addr, data);
}

void
TestShell::FullWrite(const uint32_t data)
{
    for (int addr = kMinAddr; addr <= kMaxAddr; addr++)
        Write(addr, data);
}

bool
TestShell::TestApp1()
{
    bool is_test_pass = false;

    uint32_t write_pattern = 0xABCDFFFF;
    FullWrite(write_pattern);

    vector<uint32_t> read_result = FullRead();

    is_test_pass = IsArrayDataEqual(read_result, write_pattern);
    if (is_test_pass)
        PrintOutALine(INFO, "testapp1 pass");
    else
        PrintOutALine(INFO, "testapp1 fail");

    return is_test_pass;
}

bool
TestShell::TestApp2()
{
    bool is_test_pass = false;
    uint32_t first_write_pattern = 0xAAAABBBB;
    uint32_t second_write_pattern = 0x12345678;
    uint32_t loop = 50;
    const uint32_t start_addr = 0, end_addr = 6;

    while (loop--) 
    {
        WriteRange(start_addr, end_addr, first_write_pattern);
    }

    WriteRange(start_addr, end_addr, second_write_pattern);
    
    vector<uint32_t> read_result = ReadRange(start_addr, end_addr);

    is_test_pass = IsArrayDataEqual(read_result, second_write_pattern);
    if (is_test_pass)
        PrintOutALine(INFO, "testapp2 pass");
    else
        PrintOutALine(INFO, "testapp2 fail");

    return is_test_pass;
}

void 
TestShell::WriteRange(const uint32_t start_addr, const uint32_t end_addr, const uint32_t data)
{
    for (uint32_t addr = start_addr; addr < end_addr; ++addr)
    {
        Write(addr, data);
    }
}

vector<uint32_t>
TestShell::ReadRange(const uint32_t start_addr, const uint32_t end_addr)
{
    vector<uint32_t> read_result;
    for (uint32_t addr = start_addr; addr < end_addr; ++addr)
    {
        read_result.push_back(Read(addr));
    }

    return read_result;
}

bool
TestShell::IsArrayDataEqual(vector<uint32_t> actual_arr, uint32_t expected)
{
    bool is_test_pass = true;

    for (uint32_t idx = 0; idx < actual_arr.size(); ++idx)
    {
        if (actual_arr[idx] != expected)
        {
            PrintOutALine(INFO, "idx[" + to_string(addr) + "] not equal!!");
            PrintOutALine(INFO, "actual : " + UintToHexString(actual_arr[idx]) + "expected : " + UintToHexString(expected));
            is_test_pass = false;
        }
    }

    return is_test_pass;
}

void
TestShell::Help(void)
{
    PrintOutALine(INFO, "Available commands:");
    PrintOutALine(INFO, "Write <addr> <data>: Write data to address");
    PrintOutALine(INFO, "Read <addr>: Read data from address");
    PrintOutALine(INFO, "FullWrite <data>: Write data to full address");
    PrintOutALine(INFO, "FullRead : Read data from full address");
    PrintOutALine(INFO, "Help: Show available commands");
    PrintOutALine(INFO, "Exit: Exit the program");
}

void
TestShell::PrintOutALine(const PrintLevel level, const string str)
{
    if (cur_print_level == level)
        cout << str << endl;
}

void
TestShell::PrintOutALineWithoutEndl(const PrintLevel level, const string str)
{
    if (cur_print_level == level)
        cout << str;
}

string 
TestShell::UintToHexString(const uint32_t data)
{
    ostringstream data_ss;
    data_ss << "0x" << hex << uppercase << data;

    return data_ss.str();
}

void 
TestShell::Run(void)
{
    bool isGoing = true;
    bool isCMDPass;
    while (isGoing)
    {
        isCMDPass = true;
        if (!Input()) continue;
        
        if (cmd != EXIT && cmd != HELP)
            PrintOutALineWithoutEndl(ONLY_RUNNER, str_cmd + "  ---  Run...");

        switch (cmd)
        {
            case READ :
                Read(addr);
                break;
            case FULLREAD:
                FullRead();
                break;
            case WRITE:
                Write(addr, data);
                break;
            case FULLWRITE:
                FullWrite(data);
                break;
            case HELP:
                Help();
                isCMDPass = false;
                break;
            case TESTAPP1:
                isCMDPass = TestApp1();
                break;
            case TESTAPP2:
                isCMDPass = TestApp2();
                break;
            case EXIT:
                isGoing = false;
                isCMDPass = false;
                break;
            default:
                //must not get here
                break;
        }

        if (isCMDPass)
            PrintOutALine(ONLY_RUNNER, "Pass");
        else
        {
            PrintOutALine(ONLY_RUNNER, "FAIL!");
            return;
        }
    }
}

void 
TestShell::ScriptRun(const char* script_path)
{
    if (freopen(script_path, "rt", stdin) == nullptr)
    {
        PrintOutALine(INFO, string{ script_path } + " not exist");
        return;
    }

    cur_print_level = ONLY_RUNNER;
    Run();
}

bool 
TestShell::Input(void) 
{
    const string invalid_cmd_str = "[Error] Invalid CMD";
    string user_input;
    string str_addr, str_data;

    getline(cin, user_input);
    stringstream ss(user_input);
    ss >> str_cmd;

    if (str_cmd == "Write")
    {
        cmd = WRITE;
        ss >> str_addr >> str_data;

        if (ConvertAddrToInt(str_addr) == false) return false;
        
        if (ConvertDataToInt(str_data) == false) return false;
    }
    else if (str_cmd == "FullWrite")
    {
        cmd = FULLWRITE;
        ss >> str_data;

        if (ConvertDataToInt(str_data) == false) return false;
    }
    else if (str_cmd == "Read")
    {
        cmd = READ;
        ss >> str_addr;

        if (ConvertAddrToInt(str_addr) == false) return false;
    }
    else if (str_cmd == "FullRead")
    {
        cmd = FULLREAD;
    }
    else if (str_cmd == "Help")
    {
        cmd = HELP;
    }
    else if (str_cmd == "testapp1")
    {
        cmd = TESTAPP1;
    }
    else if (str_cmd == "testapp2")
    {
        cmd = TESTAPP2;
    }
    else if (str_cmd == "Exit")
    {
        cmd = EXIT;
    }
    else
    {
        PrintOutALine(INFO, invalid_cmd_str);
        Help();
        return false;
    }

    return true;
}

bool 
TestShell::ConvertAddrToInt(const string& str_addr)
{
    if (IsInvalidAddrFormat(str_addr))
    {
        PrintOutALine(INFO, "[Error] Invalid Address");
        return false;
    }

    addr = stoi(str_addr);
    addr_arr.push_back(addr); //to be deleted

    if (IsInvalidAddrRange())
    {
        PrintOutALine(INFO, "[Error] Invalid Address");
        return false;
    }

    return true;
}

bool
TestShell::IsInvalidAddrFormat(const string& str_addr)
{
    if (str_addr.length() > kAddrLen) return true;
    if (IsDecNum(str_addr) == false) return true;

    return false;
}

bool
TestShell::IsInvalidAddrRange(void)
{
    if (addr < kMinAddr || addr > kMaxAddr) return true;

    return false;
}

bool 
TestShell::ConvertDataToInt(const string& str_data)
{
    if (IsInvalidDataFormat(str_data))
    {
        PrintOutALine(INFO, "[Error] Invalid Data");
        return false;
    }

    data = stoul(str_data, nullptr, 16);

    data_arr.push_back(data); //to be deleted
    return true;
}

bool
TestShell::IsInvalidDataFormat(const string& str_data)
{
    if (str_data.length() != kDataLen) return true;
    if (IsHexNum(str_data) == false) return true;

    return false;
}

bool 
TestShell::IsHexNum(const string& str)
{
    if(str.find("0x") != 0) return false;

    for (int idx = 2; idx < str.length(); idx++)
    {
        if (!(str[idx] >= '0' && str[idx] <= '9') &&
            !(str[idx] >= 'A' && str[idx] <= 'F'))
            return false;
    }

    return true;
}

bool 
TestShell::IsDecNum(const std::string& str)
{
    for (char digit : str)
        if (!(digit >= '0' && digit <= '9')) return false;

    return true;
}

void 
TestShell::set_ssd_app(ISSDApp* app)
{
    ssd_app = app;
}