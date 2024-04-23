#pragma once

#include"ICommand.h"
class Reader : public ICommand {
public:
    Reader(string cmd, ISSDApp* ssd_application)
    {
        shellcmd = (cmd == "Read") ? READ : FULLREAD;
        ssd_app = ssd_application;
    };
    virtual bool Parsing(stringstream& SS) override;
    virtual bool Run(void) override;
    vector<uint32_t> GetResult(void);
private:
    uint32_t addr;
    uint32_t data;
    vector<uint32_t> read_result;
};