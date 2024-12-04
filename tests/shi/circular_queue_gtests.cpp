
#include <gtest/gtest.h>

#include "shi/circular_queue.hpp"

using namespace shi;

TEST(circular_queue_test, constructor_default)
{
    circular_queue<int> q{};
    EXPECT_TRUE(q.empty());
    EXPECT_EQ(q.size(), 0);
}
