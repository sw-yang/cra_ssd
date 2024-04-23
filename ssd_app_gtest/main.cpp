#include <gtest/gtest.h>

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();

    while (1);
    return 0;
}