#include<iostream>
#include<string>
#include<sstream>
#include <cstdlib>
#include"SSD_Adaptor.h"
using namespace std;

void
SSD_Adaptor::Read(const uint32_t addr){
    string cmd = "..\\x64\\Debug\\ssd_app.exe R " + to_string(addr);
    system(cmd.c_str());
}

void
SSD_Adaptor::Write(const uint32_t addr, const uint32_t data) {
    std::stringstream ss;
    ss << hex << uppercase << data;
    string cmd = "..\\x64\\Debug\\ssd_app.exe W " + to_string(addr) + " 0x" + ss.str();
    system(cmd.c_str());
}