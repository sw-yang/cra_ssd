#pragma once
#include <stdint.h>
class ISSDApp
{
public:
    virtual uint32_t Read(const uint32_t addr) = 0;
    virtual void Write(const uint32_t addr, const uint32_t data) = 0;
    virtual void Erase(const uint32_t addr, const uint32_t size) = 0;
    virtual void Flush(void) = 0;
};
