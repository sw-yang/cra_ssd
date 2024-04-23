#pragma once

#include"ICommand.h"
class TestApp1 : public ICommand {
public:
    TestApp1(string cmd, ISSDApp* ssd_application)
    {
        shellcmd = TESTAPP1;
        ssd_app = ssd_application;
    };
    bool Parsing(stringstream& SS) override;
    bool Run(void) override;
private:
    bool is_test_pass;
};