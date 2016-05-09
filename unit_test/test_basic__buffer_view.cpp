#include <string>
#include <iostream>
#include "gtest/gtest.h"
#include "parser/basic/_.hpp"

namespace {

using namespace std;
using namespace parser_tools::basic;

TEST(test_parser_basic, buffer)
{
    std::string str("hello");
    buffer<char> b(str);
    COUT(b);
    char read_buffer[10];
    int i = 0;
    char *c;
    while (c = b.read()) {
	read_buffer[i++] = *c;
    }
    EXPECT_EQ(str, std::string(read_buffer, i));

    // unread
    b.unread('1');
    b.unread('2');
    b.unread('3');
    COUT(b);

    // unread sequence
    std::string str2("45");
    b.unread(str2.begin(), str2.end());
    COUT(b);
}

}
