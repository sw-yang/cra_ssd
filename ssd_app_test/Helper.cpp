#include "Helper.h"
using namespace std;
bool
Helper::Parsing(stringstream& ss)
{
    return true;
}
bool
Helper::Run(void)
{
    LOGGER.PrintOutALine(INFO, "Available commands:", __func__);
    LOGGER.PrintOutALine(INFO, "Write <addr> <data>: Write data to address", __func__);
    LOGGER.PrintOutALine(INFO, "Read <addr>: Read data from address", __func__);
    LOGGER.PrintOutALine(INFO, "FullWrite <data>: Write data to full address", __func__);
    LOGGER.PrintOutALine(INFO, "FullRead : Read data from full address", __func__);
    LOGGER.PrintOutALine(INFO, "Help: Show available commands", __func__);
    LOGGER.PrintOutALine(INFO, "Exit: Exit the program", __func__);
    return true;
}