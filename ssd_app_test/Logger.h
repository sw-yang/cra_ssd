#pragma once
#include <string>

enum PrintLevel
{
    INFO,
    ONLY_RUNNER,
};

class Logger {
public:
    void SetPrintLevel(const PrintLevel level);
    void PrintOutALine(const PrintLevel level, const std::string str);
    void PrintOutALineWithoutEndl(const PrintLevel level, const std::string str);
private:    
    PrintLevel cur_print_level = INFO;
    std::string file_name = "latest.log";
};