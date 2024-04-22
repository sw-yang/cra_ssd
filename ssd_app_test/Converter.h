#pragma once
#include <string>
#include "InvalidChecker.h"

using namespace std;
class Converter {
public:
    static Converter& GetConverter(void)
    {
        static Converter converter;
        return converter;
    }
    bool ConvertAddrToInt(const string& str_addr, uint32_t& addr);
    bool ConvertEraseEndAddrToInt(const string& str_endaddr, uint32_t& data);

    bool ConvertDataToInt(const std::string& str_data, uint32_t& data);
    bool ConvertEraseSizeToInt(const std::string& str_data, uint32_t& data);
private:
    InvalidChecker invalidchecker;
};