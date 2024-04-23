#include"Writer.h"
#include"Logger.h"
using namespace std;
bool
Writer::Parsing(stringstream& ss)
{
    string str_addr, str_data;
    if (shellcmd == WRITE)
    {
        ss >> str_addr >> str_data;

        if (CONVERTER.ConvertAddrToInt(str_addr, addr) == false)
        {
            LOGGER.PrintOutALine(INFO, "[Error] Invalid Address", __func__);
            return false;
        }
        if (CONVERTER.ConvertDataToInt(str_data, data) == false)
        {
            LOGGER.PrintOutALine(INFO, "[Error] Invalid Data", __func__);
            return false;
        }
    }
    else if (shellcmd == FULLWRITE)
    {
        ss >> str_data;

        if (CONVERTER.ConvertDataToInt(str_data, data) == false)
        {
            LOGGER.PrintOutALine(INFO, "[Error] Invalid Data", __func__);
            return false;
        }
    }
    return true;
}

bool
Writer::Run(void)
{
    if (shellcmd == WRITE)
    {
        ssd_app->Write(addr, data);
        LOGGER.PrintOutALine(DEBUG, UintToHexString(data), __func__);
    }
    else if (shellcmd == FULLWRITE)
    {
        for (int addr = Test_Const::kMinAddr; addr <= Test_Const::kMaxAddr; addr++)
        {
            ssd_app->Write(addr, data);
            LOGGER.PrintOutALine(DEBUG, UintToHexString(data), __func__);
        }
    }
    return true;
}