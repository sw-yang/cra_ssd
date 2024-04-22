#include "Eraser.h"
using namespace std;
bool
Eraser::Parsing(stringstream& ss)
{
    string str_addr, str_data;
    if (shellcmd == ERASE)
    {
        ss >> str_addr >> str_data;

        if (CONVERTER.ConvertAddrToInt(str_addr, addr) == false) return false;

        if (CONVERTER.ConvertEraseSizeToInt(str_data, data) == false) return false;
    }
    else if (shellcmd == ERASERANGE)
    {
        ss >> str_addr >> str_data;
        if (str_addr > str_data) return false;

        if (CONVERTER.ConvertAddrToInt(str_addr, addr) == false) return false;

        if (CONVERTER.ConvertEraseEndAddrToInt(str_data, data) == false) return false;
    }
}

bool
Eraser::Run(void)
{
    if (shellcmd == ERASE)
    {
        ssd_app->Erase(addr, data);
        LOGGER.PrintOutALine(DEBUG, UintToHexString(data), __func__);
    }
    else if (shellcmd == ERASERANGE)
    {
        for (int idx = addr; idx <= data; idx++)
        {
            ssd_app->Erase(idx, 1);
            LOGGER.PrintOutALine(DEBUG, UintToHexString(idx), __func__);
        }
    }
    return true;
}

TestShellCMD 
Eraser::GetCMD(void)
{
    return shellcmd;
}

void
Eraser::set_ssd_app(ISSDApp* app)
{
    ssd_app = app;
}