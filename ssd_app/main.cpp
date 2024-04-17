#include <iostream>
#include "../ssd_app/SSD.h"

int main(int argc, char* argv[]) {

    vector<string> args;
 /*   for (int i = 0; i < argc; ++i) 
    {
        args.push_back(argv[i]);
    }*/

    args.push_back("R");
    args.push_back("10");
    args.push_back("0x00000000");

    SSD ssd;

    try
    {
        ssd.Run(Command(args));
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    
    return 0;
}