#pragma once

#include"ICommand.h"
class Reader : public ICommand {
public:
    Reader(TestShellCMD cmd) : shellcmd(cmd) {};
    virtual bool Parsing(stringstream& SS) override;
    virtual bool Run(void) override;
    virtual TestShellCMD GetCMD(void) override;
    virtual void set_ssd_app(ISSDApp* app) override;
    vector<uint32_t> GetResult(void);
private:
    TestShellCMD shellcmd;
    uint32_t addr;
    uint32_t data;
    ISSDApp* ssd_app;
    vector<uint32_t> read_result;
};