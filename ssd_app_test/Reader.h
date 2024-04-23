#pragma once

#include"ICommand.h"
class Reader : public ICommand {
public:
    Reader(TestShellCMD cmd)
    {
        shellcmd = cmd;
    };
    virtual bool Parsing(stringstream& SS) override;
    virtual bool Run(void) override;
    vector<uint32_t> GetResult(void);
private:
    uint32_t addr;
    uint32_t data;
    vector<uint32_t> read_result;
};