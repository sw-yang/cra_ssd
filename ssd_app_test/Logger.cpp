#define _CRT_SECURE_NO_WARNINGS

#include "Logger.h"
#include <iostream>
#include <sstream>
#include <chrono>
#include <ctime>

using namespace std;

Logger::Logger() 
{
    log_fout.open(file_name, ios_base::out | ios_base::app);
}

Logger::~Logger()
{
    log_fout.close();
}

void
Logger::SetPrintLevel(const PrintLevel level)
{
    cur_print_level = level;
}

void
Logger::PrintOutALine(const PrintLevel level, const string str, const string functionname)
{
    if (cur_print_level >= level) {
        if (cur_print_level > ONLY_RUNNER) {
            PrintTime();
            string space;
            for (int spacenum = 0; spacenum < 30 - functionname.size() + 2; spacenum++) space += " ";
            cout << functionname + "()" + space + ": ";
        }
        cout << str << endl;
    }
}

void
Logger::PrintOutALineWithoutEndl(const PrintLevel level, const string str, const string functionname)
{
    if (cur_print_level >= level) {
        if (cur_print_level > ONLY_RUNNER) {
            PrintTime();
            string space;
            for (int spacenum = 0; spacenum < 30 - functionname.size() + 2; spacenum++) space += " ";
            cout << functionname + "()" + space + ": ";
        }
        cout << str;
        if (cur_print_level > ONLY_RUNNER)
            cout << endl;
    }
}

void
Logger::PrintTime(void) {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm* local_time = std::localtime(&now_time);

    std::cout << '['
        << (local_time->tm_year - 100) << '.'
        << (local_time->tm_mon + 1) << '.'
        << local_time->tm_mday << ' '
        << local_time->tm_hour << ':'
        << local_time->tm_min << "] " ;
}

	string 
Logger::getTimeStr(void)
{
    time_t temp;
    struct tm* timeinfo;
    string time_str;

    time(&temp);
    timeinfo = localtime(&temp);

    timeinfo->tm_year += 1900;
    timeinfo->tm_mon + 1;

    time_str += to_string(timeinfo->tm_year);
    if (timeinfo->tm_mon < 10)
        time_str += "0";
    time_str += to_string(timeinfo->tm_mon);
    time_str += to_string(timeinfo->tm_mday);
    time_str += "_";
    time_str += to_string(timeinfo->tm_hour);
    time_str += to_string(timeinfo->tm_min);
    time_str += to_string(timeinfo->tm_sec);

    return time_str;
}

void 
Logger::LogALine(const string str)
{
    streampos log_pos = log_fout.tellp();

    if (uint32_t(log_pos) + str.length() > kMaxSize)
    {
        log_fout.close();
        
        string backup_file_name = "until_";
        backup_file_name += getTimeStr();
        backup_file_name += ".log";
        rename(file_name.c_str(), backup_file_name.c_str());

        log_fout.open(file_name, ios_base::out | ios_base::app);
    }

    log_fout << str << endl;
}
