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
    bool ConvertAddrToInt(const std::string& str_addr);
    bool IsInvalidAddrFormat(const std::string& str_addr);
    bool IsInvalidAddrRange(void);

    bool ConvertDataToInt(const std::string& str_addr);
    bool IsInvalidDataFormat(const std::string& str_addr);

    bool IsHexNum(const std::string& str);
    bool IsDecNum(const std::string& str);

    void Read(const int addr);
    void FullRead();
    void Write(const int addr, const unsigned int data);
    void FullWrite(const unsigned int data);
    void Help(void);

    ISSDApp* ssd_app;
    TestShellCMD cmd;
    int addr;
    unsigned int data;

    const int kAddrLen = 3;
    const int kMinAddr = 0;
    const int kMaxAddr = 99;
    const int kDataLen = 10;
};