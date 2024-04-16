#include "TestShell.h"
#include <iostream>
using namespace std;

void
TestShell::Read(int addr) {
    ssd_app->Read(addr);
}
void
TestShell::Write(int addr, int data) {
    ssd_app->Write(addr, data);
}

void
TestShell::Help(void) {
    std::cout << "Available commands:" << std::endl;
    std::cout << "Write <addr> <data>: Write data to address" << std::endl;
    std::cout << "Read <addr>: Read data from address" << std::endl;
    std::cout << "FullWrite <data>: Write data to full address" << std::endl;
    std::cout << "FullRead : Read data from full address" << std::endl;
    std::cout << "Help: Show available commands" << std::endl;
    std::cout << "Exit: Exit the program" << std::endl;
}