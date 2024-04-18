#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include "SSD_Adaptor.h"

using namespace std;
uint32_t
SSD_Adaptor::Read(const uint32_t  addr)
{
    system(GetReadCMD(addr).c_str());

    string data = GetReadResult();
    cout << data << endl;
    
    return stoul(data, nullptr, 16);
}

string 
SSD_Adaptor::GetReadCMD(const uint32_t addr)
{
    const string kReadCMD = "..\\x64\\Debug\\ssd_app.exe R";
    return kReadCMD + " " + to_string(addr);
}

string
SSD_Adaptor::GetReadResult(void)
{
    string result_file_name = "result.txt";
    ifstream fin(result_file_name, std::ios::in);

    char data[11];
    fin.getline(data, sizeof(data));
    fin.close();

    return string{ data };
}

void
SSD_Adaptor::Write(const uint32_t addr, const uint32_t data) {
    system(GetWriteCMD(addr, data).c_str());
}

string
SSD_Adaptor::GetWriteCMD(const uint32_t addr, const uint32_t data)
{
    const string kWriteCMD = "..\\x64\\Debug\\ssd_app.exe W ";
    std::ostringstream data_ss;
    data_ss << "0x" << hex << uppercase << data;
    
    return kWriteCMD + " " + to_string(addr) + " " + data_ss.str();
}