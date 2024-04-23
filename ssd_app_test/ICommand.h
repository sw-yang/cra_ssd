#pragma once
#include <stdint.h> 
#include <sstream>
#include <iostream>
#include <fstream>
#include "Converter.h"
#include "Logger.h"
#include "ISSDApp.h"
#include "Test_Const.h"
#define CONVERTER Converter::GetConverter()
#define LOGGER Logger::GetLogger()
using namespace std;
enum TestShellCMD
{
    READ,
    FULLREAD,
    WRITE,
    FULLWRITE,
    ERASE,
    ERASERANGE,
    FLUSH,
    HELP,
    TESTAPP1,
    TESTAPP2,
    EXIT,
};

class ICommand
{
public:
    virtual bool Parsing(stringstream& SS) = 0;
    virtual bool Run(void) = 0;
    string UintToHexString(const uint32_t data)
    {
        ostringstream data_ss;
        data_ss << "0x" << hex << uppercase << data;

        return data_ss.str();
    }
    bool IsArrayDataEqual(const vector<uint32_t> actual_arr, const uint32_t expected)
    {
        bool is_test_pass = true;

        for (uint32_t idx = 0; idx < actual_arr.size(); ++idx)
        {
            if (actual_arr[idx] != expected)
            {
                LOGGER.PrintOutALine(INFO, "idx[" + to_string(idx) + "] not equal!!", __func__);
                LOGGER.PrintOutALine(INFO, "actual : " + UintToHexString(actual_arr[idx]) + "expected : " + UintToHexString(expected), __func__);
                is_test_pass = false;
            }
        }

        return is_test_pass;
    }
    TestShellCMD GetCMD(void)
    {
        return shellcmd;
    }
//protected:
    TestShellCMD shellcmd;
    ISSDApp* ssd_app;
};