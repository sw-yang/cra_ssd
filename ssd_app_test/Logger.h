#pragma once
#include <string>
#include <fstream>

enum PrintLevel
{
    ONLY_RUNNER,
    DEBUG,
    INFO,
};

class Logger {
public:
    Logger();
    void SetPrintLevel(const PrintLevel level);
    void PrintOutALine(const PrintLevel level, const std::string str);
    void PrintOutALineWithoutEndl(const PrintLevel level, const std::string str);
private:    
    PrintLevel cur_print_level = INFO;
    std::string file_name = "latest.log";
    std::ofstream log_fout;
};