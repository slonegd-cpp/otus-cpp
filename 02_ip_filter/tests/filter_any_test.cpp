#include "ip_filter.h"
#include <vector>
#include <string>
#include <sstream>


#define BOOST_TEST_MODULE filter_any_test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (test_filter_any_valid)
{

    std::vector<std::vector<int>> vin = {
        {46,70,29,76},
        {27,251,253,58},
        {186,46,222,194},
        {1,231,69,33},
        {46,182,19,219},
        {1,87,203,225},
        {46,70,147,26},
        {1,70,44,170},
        {186,204,34,46},
        {5,1,234,8},
        {46,70,225,39},
        {1,29,168,152},
        {5,188,216,204},
        {46,70,113,73},
        {1,1,234,8},
        {185,46,87,231}
    };

    std::vector<std::vector<int>> vout_1 = {
        {1,231,69,33},
        {1,87,203,225},
        {1,70,44,170},
        {5,1,234,8},
        {1,29,168,152},
        {1,1,234,8},
    };

    std::vector<std::vector<int>> vout_46 = {
        {46,70,29,76},
        {186,46,222,194},
        {46,182,19,219},
        {46,70,147,26},
        {186,204,34,46},
        {46,70,225,39},
        {46,70,113,73},
        {185,46,87,231}
    };

    BOOST_CHECK (filter_any(vin, 1) == vout_1);
    BOOST_CHECK (filter_any(vin, 46) == vout_46);

}

BOOST_AUTO_TEST_SUITE_END()