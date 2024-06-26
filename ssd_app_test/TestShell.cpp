#define _CRT_SECURE_NO_WARNINGS

#include "TestShell.h"
#include "Logger.h"
#include "ICommand.h"
#include "CommandFactory.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;
#define LOGGER Logger::GetLogger()

void
TestShell::PrintRunnerCMDStart(void)
{
    if (ShellCMD != EXIT && LOGGER.GetPrintLevel() == ONLY_RUNNER)
        LOGGER.PrintOutALineWithoutEndl(ONLY_RUNNER, user_input + "  ---  Run...", __func__);
}

void 
TestShell::PrintRunnerCMDEnd(bool isPassed)
{
    if (ShellCMD != EXIT && LOGGER.GetPrintLevel() == ONLY_RUNNER)
    {
        if (isPassed)
            LOGGER.PrintOutALine(ONLY_RUNNER, "Pass", __func__);
        else
        {
            LOGGER.PrintOutALine(ONLY_RUNNER, "FAIL!", __func__);
            return;
        }
    }
}

void 
TestShell::Run(void)
{
    while (ShellCMD != EXIT)
    {
        if (!CreateCommand()) continue;

        PrintRunnerCMDStart();
        bool isPassed = icmd->Run();
        PrintRunnerCMDEnd(isPassed);
    }
}

void 
TestShell::ScriptRun(const char* script_path)
{
    FILE* in_file = freopen(script_path, "rt", stdin);
    if (in_file == nullptr)
    {
        LOGGER.PrintOutALine(INFO, string{ script_path } + " not exist", __func__);
        return;
    }

    LOGGER.SetPrintLevel(ONLY_RUNNER);
    Run();

    fclose(in_file);
}

bool
TestShell::CreateCommand(void) {
    string str_cmd;
    getline(cin, user_input);
    stringstream ss(user_input);
    ss >> str_cmd;

    CommandFactory cmdfactory;
    icmd = cmdfactory.CreateCommand(str_cmd, ssd_app);
    if (icmd == NULL)
    {
        LOGGER.PrintOutALine(DEBUG, "[Error] Invalid CMD", __func__);
        icmd = cmdfactory.CreateCommand("Help", ssd_app);
        icmd->Run();
        return false; 
    }
    if (icmd->Parsing(ss) == false)
    {
        return false;
    }
    ShellCMD = icmd->GetCMD();
}

void 
TestShell::set_ssd_app(ISSDApp* app)
{
    ssd_app = app;
}