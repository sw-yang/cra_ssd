#define _CRT_SECURE_NO_WARNINGS

#include "Logger.h"
#include <iostream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <filesystem>

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
    PrintOutALineWithoutEndl(level, str + "\n", functionname);
}

void 
Logger::PrintOutALineWithoutEndl(const PrintLevel level, const string str, const string functionname)
{
    if (cur_print_level >= level) {
        if (cur_print_level > ONLY_RUNNER) {
            PrintTime();
            uint32_t empty_len = 30 - functionname.size() - 2;
            cout << functionname << "()" << setw(empty_len) << setfill(' ') << " : ";
        }
        cout << str;
    }
}

void
Logger::PrintTime(void) 
{
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
Logger::GetTimeStr(void)
{
    time_t temp;
    struct tm* timeinfo;
    string time_str;

    time(&temp);
    timeinfo = localtime(&temp);

    timeinfo->tm_year += 1900;
    timeinfo->tm_mon += 1;

    time_str += to_string(timeinfo->tm_year);
    if (timeinfo->tm_mon < 10)
        time_str += "0";
    time_str += to_string(timeinfo->tm_mon);
    time_str += to_string(timeinfo->tm_mday);
    time_str += "_";
    if (timeinfo->tm_hour < 10)
        time_str += "0";
    time_str += to_string(timeinfo->tm_hour);
    if (timeinfo->tm_min < 10)
        time_str += "0";
    time_str += to_string(timeinfo->tm_min);
    if (timeinfo->tm_sec < 10)
        time_str += "0";
    time_str += to_string(timeinfo->tm_sec);

    return time_str;
}

vector<string>
Logger::GetExistOldLog(void)
{
    vector<string> path_arr;
    for (auto& entry : filesystem::directory_iterator{ "./" })
    {
        auto _path = entry.path();
        if (_path.filename() != file_name &&
            _path.extension() == ".log" &&
            _path.string().find("until_") == 2)
        {
            auto ext_pos = _path.string().find(".log");
            auto old_log_name = _path.string().erase(ext_pos);
            if (filesystem::exists(old_log_name + string{ ".zip" }))
                continue;
            path_arr.push_back(_path.string());
        }
    }

    return path_arr;
}

void
Logger::TransformOldLogToZip(void)
{
    vector<string> old_log_path_arr = GetExistOldLog();
    for (auto& old_log_path : old_log_path_arr)
    {
        string new_file_name = old_log_path;
        new_file_name = new_file_name.erase(new_file_name.find(".log")) + string{ ".zip" };
        filesystem::rename(old_log_path, new_file_name);   
    }
}

void 
Logger::BackupLatestLog(void)
{
    log_fout.close();

    string backup_file_name = "until_";
    backup_file_name += GetTimeStr();
    backup_file_name += ".log";
    filesystem::rename(file_name, backup_file_name);

    log_fout.open(file_name, ios_base::out | ios_base::app);
}

bool
Logger::IsNeededDividingLog(const string& new_log)
{
    return uint32_t(log_fout.tellp()) + new_log.length() > kMaxSize;
}

void 
Logger::LogALine(const string str)
{
    if (IsNeededDividingLog(str))
    {
        TransformOldLogToZip();
        BackupLatestLog();
    }

    log_fout << str << endl;
}
