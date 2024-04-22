#include "Logger.h"
#include <iostream>
#include <sstream>
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
Logger::PrintOutALine(const PrintLevel level, const string str)
{
    if (cur_print_level <= level)
        cout << str << endl;
}

void
Logger::PrintOutALineWithoutEndl(const PrintLevel level, const string str)
{
    if (cur_print_level <= level)
        cout << str;
}