#include "Exiter.h"
using namespace std;
bool
Exiter::Parsing(stringstream& ss)
{
    return true;
}

bool
Exiter::Run(void)
{
    return true;
}

TestShellCMD
Exiter::GetCMD(void)
{
    return shellcmd;
}
void
Exiter::set_ssd_app(ISSDApp* app)
{
    ssd_app = app;
}