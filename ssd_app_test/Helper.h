#pragma once


#include"ICommand.h"
class Helper : public ICommand {
public:
    Helper(TestShellCMD cmd)
    {
        shellcmd = cmd;
    };
    virtual bool Parsing(stringstream& SS) override;
    virtual bool Run(void) override;
private:
    uint32_t addr;
    uint32_t data;
};