#include "Flusher.h"
using namespace std;
bool
Flusher::Parsing(stringstream& ss)
{
    return true;
}

bool
Flusher::Run(void)
{
    ssd_app->Flush();
    LOGGER.PrintOutALine(DEBUG, "FLUSH", __func__);
    return true;
}
