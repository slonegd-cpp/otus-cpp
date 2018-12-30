#include "print_ip.h"
#include <sstream>


#define BOOST_TEST_MODULE print_ip_int_test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (test_suite_main)


BOOST_AUTO_TEST_CASE (test_int)
{
    std::stringstream stream;
    print_ip(-1, stream);
    print_ip(0, stream);
    print_ip(1, stream);
    print_ip(0x100, stream);
    print_ip(0x7FFF, stream);
    print_ip(0x7FFFFFFF, stream);
    print_ip(2130706433, stream);

    BOOST_CHECK_EQUAL (stream.str(), "255.255.255.255\n"
                                     "0.0.0.0\n"
                                     "0.0.0.1\n"
                                     "0.0.1.0\n"
                                     "0.0.127.255\n"
                                     "127.255.255.255\n"
                                     "127.0.0.1\n"
    );
}



BOOST_AUTO_TEST_SUITE_END()