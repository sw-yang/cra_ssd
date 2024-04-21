#pragma once
#include<string>
using namespace std;

class InvalidChecker {
public:
    bool IsInvalidAddrFormat(const std::string& str_addr);
    bool IsInvalidAddrRange(uint32_t& addr);
    bool IsInvalidDataFormat(const std::string& str_data);
    bool IsInvalidEraseSizeFormat(const std::string& str_data);

private:
    bool IsHexNum(const std::string& str);
    bool IsDecNum(const std::string& str);

};