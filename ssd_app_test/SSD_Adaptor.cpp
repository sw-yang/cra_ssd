#include<iostream>
#include<string>
#include <cstdlib>
#include"SSD_Adaptor.h"
using namespace std;

void
SSD_Adaptor::Read(const uint32_t  addr){
    string cmd = "SSD.exe r " + to_string(addr);
    system(cmd.c_str());
}

void
SSD_Adaptor::Write(const uint32_t  addr, const uint32_t  data) {
    string cmd = "SSD.exe w " + to_string(addr) + " " + to_string(data);
    system(cmd.c_str());
}