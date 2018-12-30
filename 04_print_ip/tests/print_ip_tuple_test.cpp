#include "print_ip.h"
#include <sstream>
#include <tuple>

#define BOOST_TEST_MODULE print_ip_tupler_test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (test_tuple)
{
    std::stringstream stream;
    print_ip(std::tuple<char,char,char,char>{-1,  0,   127,  10},  stream);
    print_ip(std::tuple<int,int,int>        {-1,  127,  255},      stream);
    print_ip(std::tuple<long,long,long>     {-1,  0xFF09,  0xFFFF},stream);
    print_ip(std::tuple<short>              {0xF0FF},              stream);
    

    BOOST_CHECK_EQUAL (stream.str(), "255.0.127.10\n"
                                     "255.127.255\n"
                                     "255.9.255\n"
                                     "255\n"

    );
}


BOOST_AUTO_TEST_SUITE_END()