#include "print_ip.h"
#include <sstream>


#define BOOST_TEST_MODULE print_ip_char_test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (test_char)
{
    std::stringstream stream;
    print_ip(static_cast<char>(-1), stream);
    print_ip(static_cast<char>(10), stream);

    BOOST_CHECK_EQUAL (stream.str(), "255\n"
                                     "10\n"
    );
}

BOOST_AUTO_TEST_SUITE_END()