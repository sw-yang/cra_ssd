#pragma once
class ISSDApp
{
public:
    virtual void Write(int addr, int data) = 0;
    virtual void Read(int addr) = 0;
};
