#include <iostream>
#include "../ssd_app/SSD.h"

int main(int argc, char* argv[]) {
	if (argc <= 2) return 0;
	string mode = argv[1];
	vector<string> args;
	for (int i = 2; i < argc; ++i)
	{
		args.push_back(argv[i]);
	}

	SSD ssd;
	try
	{
		ssd.Run(mode, args);
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	return 0;
}