#pragma once
#include <string>
#include <fstream>

enum PrintLevel
{
    ONLY_RUNNER,
    DEBUG,
    INFO,
};

class Logger 
{
public:
    static Logger& GetLogger(void)
    {
        static Logger logger;
        return logger;
    }    
    void SetPrintLevel(const PrintLevel level);
    void PrintOutALine(const PrintLevel level, const std::string str, const std::string functionname = "");
    void PrintOutALineWithoutEndl(const PrintLevel level, const std::string str, const std::string functionname = "");
private:
    void PrintTime(void);
    void LogALine(const std::string str);
    PrintLevel cur_print_level = INFO;
    std::string file_name = "latest.log";
    std::ofstream log_fout;
    uint32_t kMaxSize = 1024 * 1024;
    std::string getTimeStr(void);

    Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    ~Logger();
};
