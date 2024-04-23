#pragma once


#include"ICommand.h"
class Helper : public ICommand {
public:
    Helper(string cmd, ISSDApp* ssd_application)
    {
        shellcmd = HELP;
        ssd_app = ssd_application;
    };
    virtual bool Parsing(stringstream& SS) override;
    virtual bool Run(void) override;
private:
    uint32_t addr;
    uint32_t data;
};