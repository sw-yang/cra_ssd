#include <iostream>
#include "../ssd_app/SSD.h"

int main(int argc, char* argv[]) {

    vector<string> args;
    for (int i = 1; i < argc; ++i) 
    {
        args.push_back(argv[i]);
    }

    SSD ssd;
    ssd.Run(Command(args));

    return 0;
}