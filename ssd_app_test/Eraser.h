#pragma once
#include"ICommand.h"
class Eraser : public ICommand {
public:
    Eraser(string cmd, ISSDApp* ssd_application)
    {
        shellcmd = (cmd == "Erase") ? ERASE : ERASERANGE;
        ssd_app = ssd_application;
    };
    bool Parsing(stringstream& SS) override;
    bool Run(void) override;
private:
    uint32_t addr;
    uint32_t data;
};