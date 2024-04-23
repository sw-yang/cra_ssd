#pragma once

#include"ICommand.h"
class TestApp1 : public ICommand {
public:
    TestApp1(TestShellCMD cmd)
    {
        shellcmd = cmd;
    };
    virtual bool Parsing(stringstream& SS) override;
    virtual bool Run(void) override;
private:
    uint32_t addr;
    uint32_t data;
    bool is_test_pass;
};