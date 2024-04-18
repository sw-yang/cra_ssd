#include <iostream>
#include <vector>
#include "Converter.h"

using namespace std;
bool
Converter::ConvertAddrToInt(const string& str_addr, uint32_t& addr, vector<uint32_t>& addr_arr)
{
    if (invalidchecker.IsInvalidAddrFormat(str_addr))
    {
        cout << "[Error] Invalid Address" << endl;
        return false;
    }

    addr = stoi(str_addr);
    addr_arr.push_back(addr); //to be deleted

    if (invalidchecker.IsInvalidAddrRange(addr))
    {
        cout << "[Error] Invalid Address" << endl;
        return false;
    }

    return true;
}

bool
Converter::ConvertEraseEndAddrToInt(const string& str_endaddr, uint32_t& data, vector<uint32_t>& data_arr)
{
    if (invalidchecker.IsInvalidAddrFormat(str_endaddr))
    {
        cout << "[Error] Invalid Address" << endl;
        return false;
    }

    data = stoi(str_endaddr);
    data_arr.push_back(data); //to be deleted

    if (invalidchecker.IsInvalidAddrRange(data))
    {
        cout << "[Error] Invalid Address" << endl;
        return false;
    }

    return true;
}

bool
Converter::ConvertDataToInt(const string& str_data, uint32_t& data, vector<uint32_t>& data_arr)
{
    if (invalidchecker.IsInvalidDataFormat(str_data))
    {
        cout << "[Error] Invalid Data" << endl;
        return false;
    }

    data = stoul(str_data, nullptr, 16);

    data_arr.push_back(data); //to be deleted
    return true;
}

bool
Converter::ConvertEraseSizeToInt(const string& str_data, uint32_t& data, vector<uint32_t>& data_arr)
{
    if (invalidchecker.IsInvalidEraseSizeFormat(str_data))
    {
        cout << "[Error] Invalid Data" << endl;
        return false;
    }

    data = stoul(str_data, nullptr, 16);

    data_arr.push_back(data); //to be deleted
    return true;
}
