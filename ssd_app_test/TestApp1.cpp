#include "TestApp1.h"
#include "Writer.h"
#include "Reader.h"

using namespace std;
bool
TestApp1::Parsing(stringstream& ss)
{
    return true;
}

bool 
TestApp1::Run(void)
{
    is_test_pass = false;

    Writer writer("FullWrite", ssd_app);
    stringstream ss;
    uint32_t write_pattern = 0xABCDFFFF;
    ss << UintToHexString(write_pattern);
    writer.Parsing(ss);
    writer.Run();

    Reader reader("FullRead", ssd_app);
    reader.Run();
    vector<uint32_t> read_result = reader.GetResult();

    is_test_pass = IsArrayDataEqual(read_result, write_pattern);
    if (is_test_pass)
        LOGGER.PrintOutALine(INFO, "testapp1 pass", __func__);
    else
        LOGGER.PrintOutALine(INFO, "testapp1 fail", __func__);

    return is_test_pass;
}