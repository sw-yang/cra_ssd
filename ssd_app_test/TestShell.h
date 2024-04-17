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

    void Read(const uint32_t addr);
    void FullRead();
    void Write(const uint32_t addr, const uint32_t data);
    void FullWrite(const uint32_t data);
    void Help(void);

    ISSDApp* ssd_app;
    TestShellCMD cmd;
    uint32_t addr;
    uint32_t data;

    const uint32_t kAddrLen = 3;
    const uint32_t kMinAddr = 0;
    const uint32_t kMaxAddr = 99;
    const uint32_t kDataLen = 10;
};