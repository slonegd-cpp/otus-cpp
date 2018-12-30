#include "newdelete.h"
#include "allocator.h"
#include <list>

#define BOOST_TEST_MODULE out_test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (test_suite_main)

// тест на утечку памяти
BOOST_AUTO_TEST_CASE (test_allocator_memory_leak)
{
    std::size_t preCounter = my::alloc_counter;
    {   
        const size_t reserve {3};
        MyAllocator<int, reserve> alloc;

        for (int i {0}; i < reserve; ++i) {
            int* p {nullptr};
            p = alloc.allocate(1);
            BOOST_CHECK (p != nullptr and my::alloc_counter == preCounter + 1);
        }
    }
    BOOST_CHECK_EQUAL (my::alloc_counter, preCounter);
}

BOOST_AUTO_TEST_SUITE_END()