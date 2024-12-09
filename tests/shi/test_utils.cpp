
#include "shi/test_utils.hpp"

#include <sstream>

namespace shi::test
{
void test_object_tracker::on_test_begin()
{
    auto & instance = thread_local_instance();
    instance.active_objects.clear();
}

void test_object_tracker::on_test_end()
{
    auto & instance = thread_local_instance();
    for (auto && [id, test_obj] : instance.active_objects)
    {
        std::ostringstream oss;
        oss << "object constructed at " << test_obj.filename << ":" << test_obj.linenum << " never destructed";
        EXPECT_EQ("", oss.str());
    }
    instance.active_objects.clear();
}

test_object_id test_object_tracker::track_obj(char const* filename, int linenum)
{
    auto & instance = thread_local_instance();
    auto const id = instance.next_object_id++;
    SHI_TEST_ASSERT(instance.active_objects.insert({id, test_obj{filename, linenum}}).second, 
                    "%s:%d same obj instance id %llu", filename, linenum, id);
    return id;
}

void test_object_tracker::untrack_obj(test_object_id id)
{
    auto & instance = thread_local_instance();
    SHI_TEST_ASSERT(instance.active_objects.erase(id), "instance id %llu unknown", id);
}

test_object_tracker& test_object_tracker::thread_local_instance()
{
    thread_local test_object_tracker INSTANCE{};
    return INSTANCE;
}

test_base::test_base()
{
    test_object_tracker::on_test_begin();
}

test_base::~test_base()
{
    test_object_tracker::on_test_end();
}

test_int::test_int(int x, char const* filename, int linenum)
    : value{x}
    , obj_id{test_object_tracker::track_obj(filename, linenum)}
{
}

test_int::~test_int()
{
    test_object_tracker::untrack_obj(this->obj_id);
}
}
