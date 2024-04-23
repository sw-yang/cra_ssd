#pragma once
#include <string>
#include <fstream>
#include <vector>

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
    PrintLevel GetPrintLevel(void);

    void PrintOutALine(const PrintLevel level, const std::string str, const std::string functionname = "");
    void PrintOutALineWithoutEndl(const PrintLevel level, const std::string str, const std::string functionname = "");
private:
    void PrintTime(std::ostringstream& out_ss);
    void LogALine(const PrintLevel level, const std::string str, const std::string functionname = "");
    PrintLevel cur_print_level = DEBUG;
    std::string file_name = "latest.log";
    std::ofstream log_fout;
    uint32_t kMaxSize = 1024 * 1024;
    std::string GetTimeStr(void);
    std::string MakeLogString(const PrintLevel level, const std::string str, const std::string functionname);

    bool IsNeededDividingLog(const std::string& new_log);
    void TransformOldLogToZip(void);
    void BackupLatestLog(void);
    std::vector<std::string> GetExistOldLog(void);

    Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    ~Logger();
};
