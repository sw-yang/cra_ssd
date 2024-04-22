#define _CRT_SECURE_NO_WARNINGS

#include "TestShell.h"
#include "Logger.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;
#define LOGGER Logger::GetLogger()

uint32_t
TestShell::Read(const uint32_t addr)
{
    uint32_t data = ssd_app->Read(addr);

    LOGGER.PrintOutALine(DEBUG, UintToHexString(data), __func__);

    return data;
}

vector<uint32_t>
TestShell::FullRead(void)
{
    vector<uint32_t> read_result;
    for (int addr = Test_Const::kMinAddr; addr <= Test_Const::kMaxAddr; addr++)
        read_result.push_back(Read(addr));
    
    return read_result;
}

void
TestShell::Write(const uint32_t addr, const uint32_t data)
{
    ssd_app->Write(addr, data);
    LOGGER.PrintOutALine(DEBUG, UintToHexString(data), __func__);
}

void
TestShell::FullWrite(const uint32_t data)
{
    for (int addr = Test_Const::kMinAddr; addr <= Test_Const::kMaxAddr; addr++)
    {
        Write(addr, data);
        LOGGER.PrintOutALine(DEBUG, UintToHexString(data), __func__);
    }
}

void
TestShell::Erase(const uint32_t addr, const uint32_t size)
{
    ssd_app->Erase(addr, size);
    LOGGER.PrintOutALine(DEBUG, UintToHexString(data), __func__);
}

void
TestShell::EraseRange(const uint32_t startaddr, const uint32_t endaddr)
{
    for (int addr = startaddr; addr <= endaddr; addr++)
    {
        ssd_app->Erase(addr, 1);
        LOGGER.PrintOutALine(DEBUG, UintToHexString(addr), __func__);
    }
}

void
TestShell::Flush(void)
{
    ssd_app->Flush();
    LOGGER.PrintOutALine(DEBUG, "FLUSH", __func__);
}

bool
TestShell::TestApp1()
{
    LOGGER.SetPrintLevel(ONLY_RUNNER);
    bool is_test_pass = false;

    uint32_t write_pattern = 0xABCDFFFF;
    FullWrite(write_pattern);

    vector<uint32_t> read_result = FullRead();

    is_test_pass = IsArrayDataEqual(read_result, write_pattern);
    if (is_test_pass)
        LOGGER.PrintOutALine(INFO, "testapp1 pass", __func__);
    else
        LOGGER.PrintOutALine(INFO, "testapp1 fail", __func__);

    LOGGER.SetPrintLevel(INFO);
    return is_test_pass;
}

bool
TestShell::TestApp2()
{
    LOGGER.SetPrintLevel(ONLY_RUNNER);
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
        LOGGER.PrintOutALine(INFO, "testapp2 pass", __func__);
    else
        LOGGER.PrintOutALine(INFO, "testapp2 fail", __func__);

    LOGGER.SetPrintLevel(INFO);
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
TestShell::IsArrayDataEqual(const vector<uint32_t> actual_arr, const uint32_t expected)
{
    bool is_test_pass = true;

    for (uint32_t idx = 0; idx < actual_arr.size(); ++idx)
    {
        if (actual_arr[idx] != expected)
        {
            LOGGER.PrintOutALine(INFO, "idx[" + to_string(idx) + "] not equal!!", __func__);
            LOGGER.PrintOutALine(INFO, "actual : " + UintToHexString(actual_arr[idx]) + "expected : " + UintToHexString(expected), __func__);
            is_test_pass = false;
        }
    }

    return is_test_pass;
}

string
TestShell::UintToHexString(const uint32_t data)
{
    ostringstream data_ss;
    data_ss << "0x" << hex << uppercase << data;

    return data_ss.str();
}
void
TestShell::Help(void)
{
    LOGGER.PrintOutALine(INFO, "Available commands:", __func__);
    LOGGER.PrintOutALine(INFO, "Write <addr> <data>: Write data to address", __func__);
    LOGGER.PrintOutALine(INFO, "Read <addr>: Read data from address", __func__);
    LOGGER.PrintOutALine(INFO, "FullWrite <data>: Write data to full address", __func__);
    LOGGER.PrintOutALine(INFO, "FullRead : Read data from full address", __func__);
    LOGGER.PrintOutALine(INFO, "Help: Show available commands", __func__);
    LOGGER.PrintOutALine(INFO, "Exit: Exit the program", __func__);
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
        
        if (cmd != EXIT)
            LOGGER.PrintOutALineWithoutEndl(ONLY_RUNNER, user_input + "  ---  Run...", __func__);

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
            case ERASE:
                Erase(addr, data);
                break;
            case ERASERANGE:
                EraseRange(addr, data);
                break;
            case FLUSH:
                Flush();
                break;
            case HELP:
                Help();
                break;
            case TESTAPP1:
                isCMDPass = TestApp1();
                break;
            case TESTAPP2:
                isCMDPass = TestApp2();
                break;
            case EXIT:
                isGoing = false;
                break;
            default:
                //must not get here
                break;
        }

        if (cmd != EXIT)
        {
            if (isCMDPass)
                LOGGER.PrintOutALine(ONLY_RUNNER, "Pass", __func__);
            else
            {
                LOGGER.PrintOutALine(ONLY_RUNNER, "FAIL!", __func__);
                return;
            }
        }
    }
}

void 
TestShell::ScriptRun(const char* script_path)
{
    FILE* in_file = freopen(script_path, "rt", stdin);
    if (in_file == nullptr)
    {
        LOGGER.PrintOutALine(INFO, string{ script_path } + " not exist", __func__);
        return;
    }

    LOGGER.SetPrintLevel(ONLY_RUNNER);
    Run();

    fclose(in_file);
}

bool 
TestShell::Input(void) 
{
    const string invalid_cmd_str = "[Error] Invalid CMD";
    string str_cmd, str_addr, str_data;

    getline(cin, user_input);
    stringstream ss(user_input);
    ss >> str_cmd;

    if (str_cmd == "Write")
    {
        cmd = WRITE;
        ss >> str_addr >> str_data;

        if (converter.ConvertAddrToInt(str_addr, addr, addr_arr) == false) return false;
        
        if (converter.ConvertDataToInt(str_data, data, data_arr) == false) return false;
    }
    else if (str_cmd == "FullWrite")
    {
        cmd = FULLWRITE;
        ss >> str_data;

        if (converter.ConvertDataToInt(str_data, data, data_arr) == false) return false;
    }
    else if (str_cmd == "Erase")  // Input : Erase address size
    {
        cmd = ERASE;
        ss >> str_addr >> str_data;

        if (converter.ConvertAddrToInt(str_addr, addr, addr_arr) == false) return false;

        if (converter.ConvertEraseSizeToInt(str_data, data, data_arr) == false) return false;
    }
    else if (str_cmd == "EraseRange") // Input : EraseRange startaddress endaddress
    {
        cmd = ERASERANGE;
        ss >> str_addr >> str_data;
        if (str_addr > str_data) return false;

        if (converter.ConvertAddrToInt(str_addr, addr, addr_arr) == false) return false;

        if (converter.ConvertEraseEndAddrToInt(str_data, data, data_arr) == false) return false;
    }
    else if (str_cmd == "Read")
    {
        cmd = READ;
        ss >> str_addr;

        if (converter.ConvertAddrToInt(str_addr, addr, addr_arr) == false) return false;
    }
    else if (str_cmd == "FullRead")
    {
        cmd = FULLREAD;
    }
    else if (str_cmd == "Flush")
    {
        cmd = FLUSH;
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
        LOGGER.PrintOutALine(INFO, invalid_cmd_str, __func__);
        Help();
        return false;
    }

    return true;
}

void 
TestShell::set_ssd_app(ISSDApp* app)
{
    ssd_app = app;
}
