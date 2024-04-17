#include "TestShell.h"
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

void
TestShell::Read(const int addr)
{
    ssd_app->Read(addr);
}

void
TestShell::FullRead(void)
{
    for (int addr = kMinAddr; addr <= kMaxAddr; addr++)
        ssd_app->Read(addr);
}

void
TestShell::Write(const int addr, const unsigned int data)
{
    ssd_app->Write(addr, data);
}

void
TestShell::FullWrite(const unsigned int data)
{
    for (int addr = kMinAddr; addr <= kMaxAddr; addr++)
        ssd_app->Write(addr, data);
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
            case WRITE:
                Write(addr, data);
                break;
            case FULLWRITE:
                FullWrite(data);
                break;
            case HELP:
                Help();
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

        data = stoi(str_data);
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