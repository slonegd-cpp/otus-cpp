#include "ip_out.h"
#include <vector>
#include <string>
#include <sstream>


#define BOOST_TEST_MODULE out_test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (test_out_valid)
{
    std::vector<int> v = {1, 2, 3, 4};
    std::stringstream stream;

    stream << v;
    BOOST_CHECK (stream.str() == "1.2.3.4\n");

    std::vector<int> v2 = {1, 100};
    std::stringstream stream2;
    stream2 << v2;
    BOOST_CHECK (stream2.str() == "1.100\n");

}

BOOST_AUTO_TEST_SUITE_END()