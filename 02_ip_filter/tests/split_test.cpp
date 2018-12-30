#include "split.h"

#define BOOST_TEST_MODULE split_test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (test_split_valid)
{
    // ("",  '.') -> [""]
    BOOST_CHECK (split("",  '.').at(0) == "");
    // ("11", '.') -> ["11"]
    BOOST_CHECK (split("11",  '.').at(0) == "11");
    // ("..", '.') -> ["", "", ""]
    BOOST_CHECK (split("..", '.').at(0) == "");
    BOOST_CHECK (split("..", '.').at(1) == "");
    BOOST_CHECK (split("..", '.').at(2) == "");
    // ("11.", '.') -> ["11", ""]
    BOOST_CHECK (split("11.", '.').at(0) == "11");
    BOOST_CHECK (split("11.", '.').at(1) == "");
    // (".11", '.') -> ["", "11"]
    BOOST_CHECK (split(".11", '.').at(0) == "");
    BOOST_CHECK (split(".11", '.').at(1) == "11");
    // ("11.22", '.') -> ["11", "22"]
    BOOST_CHECK (split("11.22", '.').at(0) == "11");
    BOOST_CHECK (split("11.22", '.').at(1) == "22");
}

BOOST_AUTO_TEST_SUITE_END()