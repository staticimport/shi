
#include "shi/circular_queue.hpp"
#include "shi/test_utils.hpp"

class circular_queue_test : public test_base
{
public:
    circular_queue_test() = default;
};


TEST_F(circular_queue_test, constructor_default)
{
    circular_queue<int> q{};
    EXPECT_TRUE(q.empty());
    EXPECT_EQ(q.size(), 0);

    SHI_TEST_INT(3);
}
