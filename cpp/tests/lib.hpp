#pragma once

#include <iostream>

#define ASSERT_EQUALS(x, y)                                                    \
    {                                                                          \
        auto expected = x;                                                     \
        auto actual = y;                                                       \
        if (expected != actual) {                                              \
            std::cerr << "Expected: " << std::endl                             \
                      << '\t' << expected << std::endl                         \
                      << "Got: " << std::endl                                  \
                      << '\t' << actual << std::endl;                          \
            exit(1);                                                           \
        }                                                                      \
    }
