#pragma once
#include "ISSDApp.h"
#include <string>
#include <vector>
#include "Test_Const.h"
#include "Converter.h"
#include "ICommand.h"

class TestShell
{
public:
    void Run();
    void ScriptRun(const char* script_path);
    void set_ssd_app(ISSDApp* app);
private:
    bool CreateCommand(void);
    ISSDApp* ssd_app;
    ICommand* icmd;
    std::string user_input;
};
