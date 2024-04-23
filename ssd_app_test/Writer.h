#pragma once
#include"ICommand.h"
class Writer : public ICommand {
public:
    Writer(TestShellCMD cmd)
    {
        shellcmd = cmd;
    };
    virtual bool Parsing(stringstream& SS) override;
    virtual bool Run(void) override;

private:
    uint32_t addr;
    uint32_t data;
};