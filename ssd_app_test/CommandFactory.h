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
    ICommand* CreateCommand(string cmd) {
        if (cmd == "Write" || cmd == "FullWrite")
        {
            TestShellCMD shellcmd = (cmd =="Write") ? WRITE : FULLWRITE;
            return new Writer(shellcmd);
        }
        else if (cmd == "Read" || cmd == "FullRead")
        {
            TestShellCMD shellcmd = (cmd == "Read") ? READ : FULLREAD;
            return new Reader(shellcmd);
        }
        else if (cmd == "Erase" || cmd == "EraseRange")
        {
            TestShellCMD shellcmd = (cmd =="Erase") ? ERASE : ERASERANGE;
            return new Eraser(shellcmd);
        }
        else if (cmd == "Flush")
        {
            TestShellCMD shellcmd = FLUSH;
            return new Flusher(shellcmd);
        }
        else if (cmd == "Help")
        {
            TestShellCMD shellcmd = HELP;
            return new Helper(shellcmd);
        }
        else if (cmd == "testapp1")
        {
            TestShellCMD shellcmd = TESTAPP1;
            return new TestApp1(shellcmd);
        }
        else if (cmd == "testapp2")
        {
            TestShellCMD shellcmd = TESTAPP2;
            return new TestApp2(shellcmd);
        }
        else if (cmd == "Exit") 
        {
            TestShellCMD shellcmd = EXIT;
            return new Exiter(shellcmd);
        }
        else
        {
            return NULL;
        }
    }
};