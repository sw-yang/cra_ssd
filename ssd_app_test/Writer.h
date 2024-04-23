#pragma once
#include"ICommand.h"
class Writer : public ICommand {
public:
    Writer(string cmd, ISSDApp* ssd_application)
    {
        shellcmd = (cmd == "Write") ? WRITE : FULLWRITE;
        ssd_app = ssd_application;
    };
    bool Parsing(stringstream& SS) override;
    bool Run(void) override;

private:
    uint32_t addr;
    uint32_t data;
};