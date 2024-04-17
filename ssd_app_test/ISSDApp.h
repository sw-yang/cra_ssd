#pragma once
#include <stdint.h>
class ISSDApp
{
public:
    virtual void Write(const uint32_t addr, const uint32_t data) = 0;
    virtual uint32_t Read(const uint32_t addr) = 0;
};
