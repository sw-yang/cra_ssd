#pragma once

#include"ICommand.h"
class Flusher : public ICommand {
public:
    Flusher(TestShellCMD cmd)
    {
        shellcmd = cmd;
    };
    virtual bool Parsing(stringstream& SS) override;
    virtual bool Run(void) override;
private:
    uint32_t addr;
    uint32_t data;
};