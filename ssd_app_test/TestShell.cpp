#include "TestShell.h"
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

void
TestShell::Read(int addr)
{
    ssd_app->Read(addr);
}
void
TestShell::FullRead()
{
    for (int addr = 0; addr < 100; addr++)
        ssd_app->Read(addr);
}
void
TestShell::Write(int addr, int data)
{
    ssd_app->Write(addr, data);
}
void
TestShell::FullWrite(int data)
{
    for (int addr = 0; addr < 100; addr++)
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

void TestShell::Run(void)
{
    bool isGoing = true;
    while (isGoing)
    {
        Input();
        if (!CheckValidity()) continue;

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
                break;
            case EXIT:
                isGoing = false;
                break;
            default:
                break;
        }
        
    }
}

void TestShell::Input(void) 
{
    string user_input;
    string str_cmd, str_addr, str_data;
    int buf_index = 0;
    getline(cin, user_input);

    stringstream ss(user_input);
    ss >> str_cmd;

    if (str_cmd == "Write")
    {
        cmd = WRITE;
        ss >> str_addr >> str_data;
        
        addr = stoi(str_addr);
        str_data.erase(str_data.begin(), str_data.begin() + 2);
        data = 0x111AB222;
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

        addr = stoi(str_addr);
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
}

bool TestShell::CheckValidity(void)
{
    return true;
}

void TestShell::set_ssd_app(ISSDApp* app)
{
    ssd_app = app;
}