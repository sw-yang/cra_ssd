#pragma once

#include"ICommand.h"
class TestApp1 : public ICommand {
public:
    TestApp1(string cmd, ISSDApp* ssd_application)
    {
        shellcmd = TESTAPP1;
        ssd_app = ssd_application;
    };
    virtual bool Parsing(stringstream& SS) override;
    virtual bool Run(void) override;
private:
    uint32_t addr;
    uint32_t data;
    bool is_test_pass;
};