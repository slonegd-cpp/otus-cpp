#include "print_ip.h"
#include <sstream>


#define BOOST_TEST_MODULE print_ip_long_test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (test_suite_main)


BOOST_AUTO_TEST_CASE (test_long)
{
    std::stringstream stream;
    print_ip(-1L, stream);
    print_ip(0L, stream);
    print_ip(1L, stream);
    print_ip(0x100L, stream);
    print_ip(0x7FFFL, stream);
    print_ip(0x7FFFFFFFL, stream);
    print_ip(0x7FFFFFFFFFFFFFFFL, stream);

    BOOST_CHECK_EQUAL (stream.str(), "255.255.255.255.255.255.255.255\n"
                                     "0.0.0.0.0.0.0.0\n"
                                     "0.0.0.0.0.0.0.1\n"
                                     "0.0.0.0.0.0.1.0\n"
                                     "0.0.0.0.0.0.127.255\n"
                                     "0.0.0.0.127.255.255.255\n"
                                     "127.255.255.255.255.255.255.255\n"
    );
}

BOOST_AUTO_TEST_SUITE_END()