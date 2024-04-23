#pragma once
#include"ICommand.h"
class TestApp2 : public ICommand {
public:
    TestApp2(string cmd, ISSDApp* ssd_application)
    {
        shellcmd = TESTAPP2;
        ssd_app = ssd_application;
    };
    bool Parsing(stringstream& SS) override;
    bool Run(void) override;

private:
    void WriteRange(const uint32_t start_addr, const uint32_t end_addr, const uint32_t data);
    vector<uint32_t> ReadRange(const uint32_t start_addr, const uint32_t end_addr);
    bool is_test_pass;
};