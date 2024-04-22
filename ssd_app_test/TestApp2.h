#pragma once
#include"ICommand.h"
class TestApp2 : public ICommand {
public:
    TestApp2(TestShellCMD cmd) : shellcmd(cmd) {};
    virtual bool Parsing(stringstream& SS) override;
    virtual bool Run(void) override;
    virtual TestShellCMD GetCMD(void) override;
    virtual void set_ssd_app(ISSDApp* app) override;

private:
    void WriteRange(const uint32_t start_addr, const uint32_t end_addr, const uint32_t data);
    vector<uint32_t> ReadRange(const uint32_t start_addr, const uint32_t end_addr);
    TestShellCMD shellcmd;
    uint32_t addr;
    uint32_t data;
    bool is_test_pass;
    ISSDApp* ssd_app;
};