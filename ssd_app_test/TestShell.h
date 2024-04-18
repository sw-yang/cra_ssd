#pragma once
#include "ISSDApp.h"
#include <string>
#include <vector>
#include "Test_Const.h"
#include "Converter.h"

enum TestShellCMD
{
    READ,
    FULLREAD,
    WRITE,
    FULLWRITE,
    ERASE,
    ERASERANGE,
    HELP,
    TESTAPP1,
    TESTAPP2,
    EXIT,
};

enum PrintLevel
{
    INFO,
    ONLY_RUNNER,
};

class TestShell
{
public:
    void Run();
    void ScriptRun(const char* script_path);
    void set_ssd_app(ISSDApp* app);
    ISSDApp* get_ssd_app(void) { return ssd_app; } //to be deleted
    std::vector<uint32_t> addr_arr;//to be deleted
    std::vector<uint32_t> data_arr;//to be deleted
private:
    bool Input(void);;

    uint32_t Read(const uint32_t addr);
    std::vector<std::uint32_t> FullRead(void);
    void Write(const uint32_t addr, const uint32_t data);
    void FullWrite(const uint32_t data);
    void Erase(const uint32_t addr, const uint32_t size);
    void EraseRange(const uint32_t startaddr, const uint32_t endaddr);

    void Help(void);
    bool TestApp1(void);
    bool TestApp2(void);
    void WriteRange(uint32_t start_addr, uint32_t end_addr, uint32_t data);
    std::vector<uint32_t> ReadRange(uint32_t start_addr, uint32_t end_addr);
    bool IsArrayDataEqual(const std::vector<uint32_t> actual, const uint32_t expected);
    void PrintOutALine(const PrintLevel level, const std::string str);
    void PrintOutALineWithoutEndl(const PrintLevel level, const std::string str);
    std::string UintToHexString(const uint32_t data);

    ISSDApp* ssd_app;
    TestShellCMD cmd;
    uint32_t addr;
    uint32_t data;

    PrintLevel cur_print_level = INFO;
    std::string user_input;
    Converter converter;
};