#include "Logger.h"
#include <iostream>

using namespace std;

void
Logger::SetPrintLevel(const PrintLevel level)
{
    cur_print_level = level;
}

void
Logger::PrintOutALine(const PrintLevel level, const string str)
{
    if (cur_print_level == level)
        cout << str << endl;
}

void
Logger::PrintOutALineWithoutEndl(const PrintLevel level, const string str)
{
    if (cur_print_level == level)
        cout << str;
}