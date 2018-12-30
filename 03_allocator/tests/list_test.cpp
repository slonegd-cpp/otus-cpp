/**
 *  Тесты по заданию
 *  Реализовать свой контейнер, который по аналогии с контейнерами stl
 *  параметризуется аллокатором. Контейнер должен иметь две возможности
 *  - добавить новый элемент и обойти контейнер в одном направлении.
 * 
 */
#include "newdelete.h"
#include "list.h"
#include <sstream>

#define BOOST_TEST_MODULE out_test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (test_suite_main)

// тест методов контейнера push, foreach
BOOST_AUTO_TEST_CASE (test_list_push_foreach)
{
    list<int> l;
    std::stringstream out;

    l.push(1);
    l.push(2);
    l.push(3);

    for (auto n : l)
        out << n;

    BOOST_CHECK (out.str() == "123");
}

// тест на утечку памяти
BOOST_AUTO_TEST_CASE (test_list_memory_leak)
{
    std::size_t preCounter = my::alloc_counter;
    {   
        const size_t size {3};
        list<int> l;
        for (int i {0}; i < size; ++i) {
            l.push(i);
        }
    }
    BOOST_CHECK (my::alloc_counter == preCounter);
}

BOOST_AUTO_TEST_SUITE_END()