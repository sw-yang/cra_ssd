#pragma once
#include "ISSDApp.h"

class TestShell
{
public:
    void Run()
    {
    }
    void set_ssd_app(ISSDApp* app)
    {
        ssd_app = app;
    }
    ISSDApp* get_ssd_app(void)
    {
        return ssd_app;
    }
private:
    void Input(void);
    void Read(int addr);
    void Write(int addr, int data);
    void Help(void);
    ISSDApp* ssd_app;
};