#include "print_ip.h"
#include <sstream>

using namespace std::literals;


#define BOOST_TEST_MODULE print_ip_string_test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (test_string)
{
    std::stringstream stream;
    print_ip("127.255.255.0"s, stream);
    print_ip("10"s, stream);
    print_ip("All that you wish"s, stream);
    print_ip("static_cast<char>(10)"s, stream);
    

    BOOST_CHECK_EQUAL (stream.str(), "127.255.255.0\n"
                                     "10\n"
                                     "All that you wish\n"
                                     "static_cast<char>(10)\n"
    );
}

BOOST_AUTO_TEST_SUITE_END()