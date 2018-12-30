#include "ip_filter.h"
#include "ip_out.h"
#include <vector>
#include <string>
#include <sstream>


#define BOOST_TEST_MODULE filter_test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (test_filter_valid)
{

    std::vector<std::vector<int>> vin = {
        {46,70,29,76},
        {27,251,253,58},
        {1,231,69,33},
        {46,182,19,219},
        {1,87,203,225},
        {46,70,147,26},
        {1,70,44,170},
        {5,1,234,8},
        {46,70,225,39},
        {1,29,168,152},
        {5,188,216,204},
        {46,70,113,73},
        {1,1,234,8}
    };

    std::vector<std::vector<int>> vout_1 = {
        {1,231,69,33},
        {1,87,203,225},
        {1,70,44,170},
        {1,29,168,152},
        {1,1,234,8}
    };

    std::vector<std::vector<int>> vout_46_70 = {
        {46,70,29,76},
        {46,70,147,26},
        {46,70,225,39},
        {46,70,113,73},
    };

    BOOST_CHECK (filter(vin, 1)      == vout_1);
    BOOST_CHECK (filter(vin, 46, 70) == vout_46_70);

}

BOOST_AUTO_TEST_SUITE_END()