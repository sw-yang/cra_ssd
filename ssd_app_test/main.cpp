#include "TestShell.h"
#include "SSD_Adaptor.h"

int main(void) {
	SSD_Adaptor ssd_app;
	TestShell shell;
	shell.set_ssd_app(&ssd_app);

	shell.Run();

	return 0;
}