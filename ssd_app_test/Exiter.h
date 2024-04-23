#pragma once
#include"ICommand.h"
class Exiter : public ICommand {
public:
    Exiter(string cmd, ISSDApp* ssd_application)
    {
        shellcmd = EXIT;
        ssd_app = ssd_application;
    };
    bool Parsing(stringstream& SS) override;
    bool Run(void) override;
};