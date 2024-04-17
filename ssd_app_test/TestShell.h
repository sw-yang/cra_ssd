#pragma once
#include "ISSDApp.h"
#include <string>
#include <vector> //to be deleted

enum TestShellCMD
{
    READ,
    FULLREAD,
    WRITE,
    FULLWRITE,
    HELP,
    EXIT,
};

class TestShell
{
public:
    void Run();
    void set_ssd_app(ISSDApp* app);
    ISSDApp* get_ssd_app(void) { return ssd_app; } //to be deleted
    std::vector<int> addr_arr;//to be deleted
    std::vector<int> data_arr;//to be deleted
private:
    bool Input(void);
    bool ConvertAddrToInt(std::string& str_addr);
    bool ConvertDataToInt(std::string& str_addr);
    bool IsHexNum(std::string& str);
    void Read(int addr);
    void FullRead();
    void Write(int addr, int data);
    void FullWrite(int data);
    void Help(void);
    ISSDApp* ssd_app;
    TestShellCMD cmd;
    int addr;
    int data;
};