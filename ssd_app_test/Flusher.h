#pragma once

#include"ICommand.h"
class Flusher : public ICommand {
public:
    Flusher(string cmd, ISSDApp* ssd_application)
    {
        shellcmd = FLUSH;
        ssd_app = ssd_application;
    };
    bool Parsing(stringstream& SS) override;
    bool Run(void) override;
};