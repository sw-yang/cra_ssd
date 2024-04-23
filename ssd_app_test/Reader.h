#pragma once

#include"ICommand.h"
class Reader : public ICommand {
public:
    Reader(string cmd, ISSDApp* ssd_application)
    {
        shellcmd = (cmd == "Read") ? READ : FULLREAD;
        ssd_app = ssd_application;
    };
    bool Parsing(stringstream& SS) override;
    bool Run(void) override;
    vector<uint32_t> GetResult(void);
private:
    uint32_t addr;
    vector<uint32_t> read_result;
};