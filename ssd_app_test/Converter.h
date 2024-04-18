#pragma once
#include <string>
#include "InvalidChecker.h"

using namespace std;
class Converter {
public:
    bool ConvertAddrToInt(const string& str_addr, uint32_t& addr, vector<uint32_t>& addr_arr);
    bool ConvertEraseEndAddrToInt(const string& str_endaddr, uint32_t& data, vector<uint32_t>& data_arr);

    bool ConvertDataToInt(const std::string& str_data, uint32_t& data, vector<uint32_t>& data_arr);
    bool ConvertEraseSizeToInt(const std::string& str_data, uint32_t& data, vector<uint32_t>& data_arr);
private:
    InvalidChecker invalidchecker;
};