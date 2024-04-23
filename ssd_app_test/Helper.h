#pragma once


#include"ICommand.h"
class Helper : public ICommand {
public:
    Helper(string cmd, ISSDApp* ssd_application)
    {
        shellcmd = HELP;
        ssd_app = ssd_application;
    };
    bool Parsing(stringstream& SS) override;
    bool Run(void) override;
};