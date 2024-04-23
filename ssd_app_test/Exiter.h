#pragma once
#include"ICommand.h"
class Exiter : public ICommand {
public:
    Exiter(TestShellCMD cmd)
    {
        shellcmd = cmd;
    };
    virtual bool Parsing(stringstream& SS) override;
    virtual bool Run(void) override;
private:
    uint32_t addr;
    uint32_t data;
};