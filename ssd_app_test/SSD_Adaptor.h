#pragma once
#include "ISSDApp.h"
class SSD_Adaptor : public ISSDApp {
public:
    void Read(const uint32_t addr) override;
    void Write(const uint32_t addr, const uint32_t data) override;
};