#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include "SSD_Adaptor.h"

using namespace std;

void
SSD_Adaptor::Read(const uint32_t addr)
{
    string cmd = "..\\x64\\Debug\\ssd_app.exe R " + to_string(addr);
    system(cmd.c_str());
    string result_file_name = "result.txt";
    ifstream fin(result_file_name, std::ios::in);
    if (!fin.is_open())
    {
        cout << result_file_name << " not exist" << endl;
        return;
    }

    char data[11];
    fin.getline(data, sizeof(data));
    cout << data << endl;
}

void
SSD_Adaptor::Write(const uint32_t addr, const uint32_t data) 
{
    std::ostringstream data_ss;
    data_ss << "0x" << hex << uppercase << data;
    
    string cmd = "..\\x64\\Debug\\ssd_app.exe W " + to_string(addr) + " " + data_ss.str();
    system(cmd.c_str());
}