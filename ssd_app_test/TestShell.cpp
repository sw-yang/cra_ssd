#include "TestShell.h"
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

void
TestShell::Read(int addr) {
    ssd_app->Read(addr);
}
void
TestShell::Write(int addr, int data) {
    ssd_app->Write(addr, data);
}

void
TestShell::Help(void) {
    std::cout << "Available commands:" << std::endl;
    std::cout << "Write <addr> <data>: Write data to address" << std::endl;
    std::cout << "Read <addr>: Read data from address" << std::endl;
    std::cout << "FullWrite <data>: Write data to full address" << std::endl;
    std::cout << "FullRead : Read data from full address" << std::endl;
    std::cout << "Help: Show available commands" << std::endl;
    std::cout << "Exit: Exit the program" << std::endl;
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
            break;
        case WRITE:
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
        data = stoi(str_data);
    }
    else if (str_cmd == "Read")
    {
        cmd = READ;
        ss >> str_addr;

        addr = stoi(str_addr);
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

ISSDApp* TestShell::get_ssd_app(void)
{
    return ssd_app;
}