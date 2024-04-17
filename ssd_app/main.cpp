#include <iostream>
#include "../ssd_app/SSD.h"

int main(int argc, char* argv[]) {
    cout << "num of arguments" << argc << endl;

    vector<string> cmd;

    cout << "arguments " << endl;
    for (int i = 0; i < argc; ++i) {
        cmd.push_back(argv[i]);
        cout << "argument " << i << ": " << argv[i] << endl;
    }

    for (auto str : cmd) {
        cout << str << endl;
    }

    return 0;
}