#include<iostream>
#include<string>
#include <cstdlib>
#include"SSD_Adaptor.h"
using namespace std;

void
SSD_Adaptor::Read(int addr){
    string cmd = "ssd_app.exe R " + to_string(addr);
    system(cmd.c_str());
}

void
SSD_Adaptor::Write(int addr, int data) {
    string cmd = "ssd_app.exe W " + to_string(addr) + " " + to_string(data);
    system(cmd.c_str());
}