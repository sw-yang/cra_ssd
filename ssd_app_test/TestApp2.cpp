#include "TestApp2.h"
#include "Writer.h"
#include "Reader.h"

using namespace std;
bool
TestApp2::Parsing(stringstream& ss)
{
    return true;
}
bool
TestApp2::Run(void)
{
    LOGGER.SetPrintLevel(ONLY_RUNNER);
    is_test_pass = false;
    uint32_t first_write_pattern = 0xAAAABBBB;
    uint32_t second_write_pattern = 0x12345678;
    uint32_t loop = 50;
    const uint32_t start_addr = 0, end_addr = 6;

    while (loop--)
    {
        WriteRange(start_addr, end_addr, first_write_pattern);
    }

    WriteRange(start_addr, end_addr, second_write_pattern);

    vector<uint32_t> read_result = ReadRange(start_addr, end_addr);
    is_test_pass = IsArrayDataEqual(read_result, second_write_pattern);
    if (is_test_pass)
        LOGGER.PrintOutALine(INFO, "testapp2 pass", __func__);
    else
        LOGGER.PrintOutALine(INFO, "testapp2 fail", __func__);

    LOGGER.SetPrintLevel(INFO);
    return is_test_pass;
}

void
TestApp2::WriteRange(const uint32_t start_addr, const uint32_t end_addr, const uint32_t data)
{
    for (uint32_t addr = start_addr; addr < end_addr; ++addr)
    {
        Writer writer("Write", ssd_app);
        stringstream ss;
        ss << to_string(addr)<< " " << UintToHexString(data);
        writer.Parsing(ss);
        writer.Run();
    }
}

vector<uint32_t>
TestApp2::ReadRange(const uint32_t start_addr, const uint32_t end_addr)
{
    Reader reader("Read", ssd_app);
    for (uint32_t addr = start_addr; addr < end_addr; ++addr)
    {
        stringstream ss;
        ss << to_string(addr);
        reader.Parsing(ss);
        reader.Run();
    }

    return reader.GetResult();
}