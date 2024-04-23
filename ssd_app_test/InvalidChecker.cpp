#include "InvalidChecker.h"
#include "Test_Const.h"

bool
InvalidChecker::IsInvalidAddrFormat(const string& str_addr)
{
    if (str_addr.length() > Test_Const::kAddrLen) return true;
    if (IsDecNum(str_addr) == false) return true;

    return false;
}

bool
InvalidChecker::IsInvalidAddrRange(uint32_t& addr)
{
    if (addr < Test_Const::kMinAddr || addr > Test_Const::kMaxAddr) return true;

    return false;
}

bool
InvalidChecker::IsInvalidDataFormat(const string& str_data)
{
    if (str_data.length() != Test_Const::kDataLen) return true;
    if (IsHexNum(str_data) == false) return true;

    return false;
}

bool
InvalidChecker::IsInvalidEraseSizeFormat(const string& str_data)
{
    if (IsDecNum(str_data) == false) return true;

    return false;
}

bool
InvalidChecker::IsHexNum(const string& str)
{
    if (str.find("0x") != 0) return false;

    for (int idx = 2; idx < str.length(); idx++)
    {
        if (!(str[idx] >= '0' && str[idx] <= '9') &&
            !(str[idx] >= 'A' && str[idx] <= 'F') &&
            !(str[idx] >= 'a' && str[idx] <= 'f'))
            return false;
    }

    return true;
}

bool
InvalidChecker::IsDecNum(const std::string& str)
{
    for (char digit : str)
        if (!(digit >= '0' && digit <= '9')) return false;

    return true;
}