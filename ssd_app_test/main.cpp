#include "TestShell.h"
#include "SSD_Adaptor.h"

int main(int argc, char* argv[]) {
	SSD_Adaptor ssd_app;
	TestShell shell;
	shell.set_ssd_app(&ssd_app);

	if (argc == 1)
		shell.Run();
	else
		shell.ScriptRun(argv[1]);

	return 0;
}