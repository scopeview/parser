#include <string>
#include <iostream>
#include "gtest/gtest.h"
#include "parser.hpp"

using namespace std;

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
