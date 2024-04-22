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

TestShellCMD
Flusher::GetCMD(void)
{
    return shellcmd;
}
void
Flusher::set_ssd_app(ISSDApp* app)
{
    ssd_app = app;
}