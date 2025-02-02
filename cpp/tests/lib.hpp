#pragma once

#include <iostream>

#define ASSERT_EQUALS(expected, actual)                                        \
    {                                                                          \
        if (expected != actual) {                                              \
            std::cerr << "Expected: " << std::endl                             \
                      << '\t' << expected << std::endl                         \
                      << "Got: " << std::endl                                  \
                      << '\t' << actual << std::endl;                          \
            exit(1);                                                           \
        }                                                                      \
    }
