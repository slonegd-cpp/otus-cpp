#include "print_ip.h"
#include <sstream>


#define BOOST_TEST_MODULE print_ip_short_test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (test_short)
{
    std::stringstream stream;
    print_ip(static_cast<short>(-1), stream);
    print_ip(static_cast<short>(0), stream);
    print_ip(static_cast<short>(1), stream);
    print_ip(static_cast<short>(0x100), stream);
    print_ip(static_cast<short>(0x7FFF), stream);

    BOOST_CHECK_EQUAL (stream.str(), "255.255\n"
                                     "0.0\n"
                                     "0.1\n"
                                     "1.0\n"
                                     "127.255\n"
    );
}

BOOST_AUTO_TEST_SUITE_END()