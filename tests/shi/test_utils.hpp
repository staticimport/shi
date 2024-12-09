#pragma once

#include <gtest/gtest.h>

#include <cstdio>

using namespace ::testing;

namespace shi::test
{
using test_object_id = uint64_t;

class test_object_tracker
{
public:
    static void           on_test_begin();
    static void           on_test_end();
    static test_object_id track_obj(char const* filename, int linenum);
    static void           untrack_obj(test_object_id);

private:
    struct test_obj
    {
        char const* filename;
        int linenum;
    };

    static test_object_tracker & thread_local_instance();

private:
    test_object_id next_object_id{};
    std::map<test_object_id, test_obj> active_objects;
};

class test_base : public ::testing::Test
{
protected:
    test_base();
    ~test_base();
};

class test_int
{
public:
    test_int(int x, char const* filename, int linenum);
    ~test_int();

private:
    int value;
    test_object_id obj_id;
};
}

using namespace shi;
using namespace shi::test;

#define SHI_TEST_INT(x) test_int{x, __FILE__, __LINE__}

#define SHI_TEST_ASSERT(check, ...) \
        do { \
            if (!(check)) { \
                fprintf(stderr, "Check `%s` failed.", #check); \
                fprintf(stderr, __VA_ARGS__); \
                fputc('\n', stderr); \
                abort();  \
            } \
        } while (false)

