#pragma once
#include "ISSDApp.h"

enum TestShellCMD
{
    READ,
    FULLREAD,
    WRITE,
    FULLWRITE,
    HELP,
    EXIT,
};

class TestShell
{
public:
    void Run();
    void set_ssd_app(ISSDApp* app);
    ISSDApp* get_ssd_app(void);
private:
    void Input(void);
    bool CheckValidity(void);
    void Read(int addr);
    void FullRead();
    void Write(int addr, int data);
    void FullWrite(int data);
    void Help(void);
    ISSDApp* ssd_app;
    TestShellCMD cmd;
    int addr;
    int data;
};