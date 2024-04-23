#pragma once
#include "ICommand.h"
#include "Writer.h"
#include "Reader.h"
#include "Eraser.h"
#include "Flusher.h"
#include "Helper.h"
#include "TestApp1.h"
#include "TestApp2.h"
#include "Exiter.h"
#include<string>
using namespace std;

class CommandFactory {
public:
    ICommand* CreateCommand(string cmd, ISSDApp* ssd_app) {
        if (cmd == "Write" || cmd == "FullWrite")
            return new Writer(cmd, ssd_app);
        else if (cmd == "Read" || cmd == "FullRead")
            return new Reader(cmd, ssd_app);
        else if (cmd == "Erase" || cmd == "EraseRange")
            return new Eraser(cmd, ssd_app);
        else if (cmd == "Flush")
            return new Flusher(cmd, ssd_app);
        else if (cmd == "Help")
            return new Helper(cmd, ssd_app);
        else if (cmd == "testapp1")
            return new TestApp1(cmd, ssd_app);
        else if (cmd == "testapp2")
            return new TestApp2(cmd, ssd_app);
        else if (cmd == "Exit") 
            return new Exiter(cmd, ssd_app);
        else
            return NULL;
    }
};