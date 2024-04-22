#pragma once
#include"ICommand.h"
class Writer : public ICommand {
public:
    Writer(TestShellCMD cmd) : shellcmd(cmd), addr(0), data(0), ssd_app(nullptr) {};
    virtual bool Parsing(stringstream& SS) override;
    virtual bool Run(void) override;
    virtual TestShellCMD GetCMD(void) override;
    virtual void set_ssd_app(ISSDApp* app) override;

private:
    TestShellCMD shellcmd;
    uint32_t addr;
    uint32_t data;
    ISSDApp* ssd_app;
};