#include <iostream>
#include "../ssd_app/SSD.h"

int main(int argc, char* argv[]) {

	if (argc <= 1) return 0;
	vector<string> args;
	for (int i = 1; i < argc; ++i)
	{
		args.push_back(argv[i]);
	}

	SSD ssd;
	Command cmd(args);
	try
	{
		ssd.Run(cmd);
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	return 0;
}