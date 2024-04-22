#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include "SSD_Adaptor.h"
#include <iomanip>

using namespace std;
uint32_t
SSD_Adaptor::Read(const uint32_t  addr)
{
    system(GetReadCMD(addr).c_str());
    return stoul(GetReadResult(), nullptr, 16);
}

string 
SSD_Adaptor::GetReadCMD(const uint32_t addr)
{
    const string kReadCMD = ssd_app_path + " R";
    return kReadCMD + " " + to_string(addr);
}

string
SSD_Adaptor::GetReadResult(void)
{
    string result_file_name = "result.txt";
    ifstream fin(result_file_name, ios::in);

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
    const string kWriteCMD = ssd_app_path + " W ";
    ostringstream data_ss;
    data_ss << "0x" << setfill('0') << hex << setw(8) << uppercase << data;
    auto ret = kWriteCMD + " " + to_string(addr) + " " + data_ss.str();
    return ret;
}

void
SSD_Adaptor::Erase(const uint32_t addr, const uint32_t size) {
    system(GetEraseCMD(addr, size).c_str());
}

string
SSD_Adaptor::GetEraseCMD(const uint32_t addr, const uint32_t size)
{
    const string kWriteCMD = ssd_app_path + " E ";
    return kWriteCMD + " " + to_string(addr) + " " + to_string(size);
}

void
SSD_Adaptor::Flush(void)
{
    system((ssd_app_path + " F").c_str());
}