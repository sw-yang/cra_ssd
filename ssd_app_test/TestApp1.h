#pragma once

#include"ICommand.h"
class TestApp1 : public ICommand {
public:
    TestApp1(TestShellCMD cmd) : shellcmd(cmd) {};
    virtual bool Parsing(stringstream& SS) override;
    virtual bool Run(void) override;
    virtual TestShellCMD GetCMD(void) override;
    virtual void set_ssd_app(ISSDApp* app) override;
private:
    TestShellCMD shellcmd;
    uint32_t addr;
    uint32_t data;
    bool is_test_pass;
    ISSDApp* ssd_app;
};