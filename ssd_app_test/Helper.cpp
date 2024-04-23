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
    LOGGER.PrintOutALine(DEBUG, "Available commands:", __func__);
    LOGGER.PrintOutALine(DEBUG, "Write <addr> <data>: Write data to address", __func__);
    LOGGER.PrintOutALine(DEBUG, "Read <addr>: Read data from address", __func__);
    LOGGER.PrintOutALine(DEBUG, "Erase <addr> <range> : Erase a range of data from addr", __func__);
    LOGGER.PrintOutALine(DEBUG, "Flush : Flush wirte buffer", __func__);
    LOGGER.PrintOutALine(DEBUG, "FullWrite <data>: Write data to full address", __func__);
    LOGGER.PrintOutALine(DEBUG, "FullRead : Read data from full address", __func__);
    LOGGER.PrintOutALine(DEBUG, "Help: Show available commands", __func__);
    LOGGER.PrintOutALine(DEBUG, "Exit: Exit the program", __func__);
    return true;
}