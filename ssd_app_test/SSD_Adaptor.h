#pragma once
#include "ISSDApp.h"
class SSD_Adaptor : public ISSDApp {
public:
    uint32_t Read(const uint32_t addr) override;
    void Write(const uint32_t addr, const uint32_t data) override;
    void Erase(const uint32_t addr, const uint32_t size) override;
    void Flush(void) override;
private:
    std::string ssd_app_path = "ssd_app.exe";
    std::string GetReadCMD(const uint32_t addr);
    std::string GetWriteCMD(const uint32_t addr, const uint32_t data);
    std::string GetEraseCMD(const uint32_t addr, const uint32_t size);
    std::string GetReadResult(void);
};