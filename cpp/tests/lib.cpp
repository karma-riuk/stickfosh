#pragma once

#define ASSERT_EQUALS(expected, actual)                                        \
    {                                                                          \
        if (expected != actual)                                                \
            std::cout << "Expected: " << std::endl                             \
                      << '\t' << expected << std::endl                         \
                      << "Got: " << std::endl                                  \
                      << '\t' << actual << std::endl;                          \
    }
