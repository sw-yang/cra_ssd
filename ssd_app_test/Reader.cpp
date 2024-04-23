#include "Reader.h"
using namespace std;
bool
Reader::Parsing(stringstream& ss)
{
    string str_addr, str_data;
    if (shellcmd == READ)
    {
        ss >> str_addr;

        if (CONVERTER.ConvertAddrToInt(str_addr, addr) == false)
        {
            LOGGER.PrintOutALine(DEBUG, "[Error] Invalid Address", __func__);
            return false;
        }
    }
    else if (shellcmd == FULLREAD)
    {
    }
}
bool
Reader::Run(void)
{
    uint32_t readdata;
    if (shellcmd == READ)
    {
        readdata = ssd_app->Read(addr);

        LOGGER.PrintOutALine(DEBUG, UintToHexString(readdata), __func__);
        read_result.push_back(readdata);
    }
    else if (shellcmd == FULLREAD)
    {
        vector<uint32_t> read_result;
        for (int addr = Test_Const::kMinAddr; addr <= Test_Const::kMaxAddr; addr++)
        {
            readdata = ssd_app->Read(addr);
            read_result.push_back(readdata);
            LOGGER.PrintOutALine(DEBUG, UintToHexString(readdata), __func__);
        }
    }
    return true;
}

vector<uint32_t>
Reader::GetResult(void)
{
    vector<uint32_t> ret = read_result;
    read_result.clear();
    return read_result;
}