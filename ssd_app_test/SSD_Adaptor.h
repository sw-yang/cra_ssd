#pragma once
#include "ISSDApp.h"
class SSD_Adaptor : public ISSDApp {
public:
    void Read(int addr) override;
    void Write(int addr, int data) override;
};