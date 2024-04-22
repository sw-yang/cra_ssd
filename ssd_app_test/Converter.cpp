#include <iostream>
#include <vector>
#include "Converter.h"
#include "ICommand.h"

using namespace std;
bool
Converter::ConvertAddrToInt(const string& str_addr, uint32_t& addr)
{
    if (invalidchecker.IsInvalidAddrFormat(str_addr))
    {
        LOGGER.PrintOutALine(INFO, "[Error] Invalid Address", __func__);
        return false;
    }

    addr = stoi(str_addr);

    if (invalidchecker.IsInvalidAddrRange(addr))
    {
        LOGGER.PrintOutALine(INFO, "[Error] Invalid Address", __func__);
        return false;
    }

    return true;
}

bool
Converter::ConvertEraseEndAddrToInt(const string& str_endaddr, uint32_t& data)
{
    if (invalidchecker.IsInvalidAddrFormat(str_endaddr))
    {
        LOGGER.PrintOutALine(INFO, "[Error] Invalid Address", __func__);
        return false;
    }

    data = stoi(str_endaddr);

    if (invalidchecker.IsInvalidAddrRange(data))
    {
        LOGGER.PrintOutALine(INFO, "[Error] Invalid Address", __func__);
        return false;
    }

    return true;
}

bool
Converter::ConvertDataToInt(const string& str_data, uint32_t& data)
{
    if (invalidchecker.IsInvalidDataFormat(str_data))
    {
        LOGGER.PrintOutALine(INFO, "[Error] Invalid Data", __func__);
        return false;
    }

    data = stoul(str_data, nullptr, 16);

    return true;
}

bool
Converter::ConvertEraseSizeToInt(const string& str_data, uint32_t& data)
{
    if (invalidchecker.IsInvalidEraseSizeFormat(str_data))
    {
        LOGGER.PrintOutALine(INFO, "[Error] Invalid Data", __func__);
        return false;
    }

    data = stoul(str_data, nullptr, 16);

    return true;
}
