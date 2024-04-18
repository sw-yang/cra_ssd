#include "TestShell.h"
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

uint32_t
TestShell::Read(const uint32_t addr)
{
    return ssd_app->Read(addr);
}

vector<uint32_t>
TestShell::FullRead(void)
{
    vector<uint32_t> read_result;
    for (int addr = kMinAddr; addr <= kMaxAddr; addr++)
        read_result.push_back(ssd_app->Read(addr));
    
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
        ssd_app->Write(addr, data);
}

void
TestShell::Erase(const uint32_t addr, const uint32_t size)
{
    ssd_app->Erase(addr, size);
}

void
TestShell::EraseRange(const uint32_t startaddr, const uint32_t endaddr)
{
    for (int addr = startaddr; addr <= endaddr; addr++)
        ssd_app->Erase(addr, 1);
}

void
TestShell::TestApp1()
{
    bool is_test_pass = true;

    uint32_t write_pattern = 0xABCDFFFF;
    FullWrite(write_pattern);

    vector<uint32_t> read_result = FullRead();

    if (IsDataEqual(read_result, write_pattern))
        cout << "testapp1 pass" << endl;
    else
        cout << "testapp1 fail" << endl;
}

void
TestShell::TestApp2()
{
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

    if (IsDataEqual(read_result, second_write_pattern))
        cout << "testapp2 pass" << endl;
    else
        cout << "testapp2 fail" << endl;
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
TestShell::IsDataEqual(vector<uint32_t> actual, uint32_t expected)
{
    bool is_test_pass = true;

    for (uint32_t idx = 0; idx < actual.size(); ++idx)
    {
        if (actual[idx] != expected)
        {
            cout << "idx : " << uppercase << hex << addr << " not equal!!" << endl;
            is_test_pass = false;
        }
    }

    return is_test_pass;
}

void
TestShell::Help(void)
{
   cout << "Available commands:" << endl;
   cout << "Write <addr> <data>: Write data to address" << endl;
   cout << "Read <addr>: Read data from address" << endl;
   cout << "FullWrite <data>: Write data to full address" << endl;
   cout << "FullRead : Read data from full address" << endl;
   cout << "Help: Show available commands" << endl;
   cout << "Exit: Exit the program" << endl;
}

void 
TestShell::Run(void)
{
    bool isGoing = true;
    while (isGoing)
    {
        if (!Input()) continue;

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
            case HELP:
                Help();
                break;
            case TESTAPP1:
                TestApp1();
                break;
            case TESTAPP2:
                TestApp2();
                break;
            case EXIT:
                isGoing = false;
                break;
            default:
                //must not get here
                break;
        }
    }
}

bool 
TestShell::Input(void) 
{
    const string invalid_cmd_str = "[Error] Invalid CMD";
    string user_input;
    string str_cmd, str_addr, str_data;

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
    else if (str_cmd == "Erase")  // Input : Erase address size
    {
        cmd = ERASE;
        ss >> str_addr >> str_data;

        if (ConvertAddrToInt(str_addr) == false) return false;

        if (ConvertEraseSizeToInt(str_data) == false) return false;
    }
    else if (str_cmd == "EraseRange") // Input : EraseRange startaddress endaddress
    {
        cmd = ERASERANGE;
        ss >> str_addr >> str_data;
        if (str_addr > str_data) return false;

        if (ConvertAddrToInt(str_addr) == false) return false;

        if (ConvertEraseEndAddrToInt(str_data) == false) return false;
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
        cout << invalid_cmd_str << endl;
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
        cout << "[Error] Invalid Address" << endl;
        return false;
    }

    addr = stoi(str_addr);
    addr_arr.push_back(addr); //to be deleted

    if (IsInvalidAddrRange())
    {
        cout << "[Error] Invalid Address" << endl;
        return false;
    }

    return true;
}

bool
TestShell::ConvertEraseEndAddrToInt(const string& str_endaddr)
{
    if (IsInvalidAddrFormat(str_endaddr))
    {
        cout << "[Error] Invalid Address" << endl;
        return false;
    }

    data = stoi(str_endaddr);
    data_arr.push_back(data); //to be deleted

    if (IsInvalidAddrRange())
    {
        cout << "[Error] Invalid Address" << endl;
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
        cout << "[Error] Invalid Data" << endl;
        return false;
    }

    data = stoul(str_data, nullptr, 16);

    data_arr.push_back(data); //to be deleted
    return true;
}

bool
TestShell::ConvertEraseSizeToInt(const string& str_data)
{
    if (IsInvalidEraseSizeFormat(str_data))
    {
        cout << "[Error] Invalid Data" << endl;
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
TestShell::IsInvalidEraseSizeFormat(const string& str_data)
{
    if (str_data.length() != 1) return true;
    if (IsDecNum(str_data) == false) return true;

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