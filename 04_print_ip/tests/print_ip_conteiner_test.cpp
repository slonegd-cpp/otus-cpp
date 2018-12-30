#include "print_ip.h"
#include <sstream>
#include <vector>
#include <list>

#define BOOST_TEST_MODULE print_ip_conteiner_test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (test_conteiner)
{
    std::stringstream stream;
    print_ip(std::vector<char>{-1,  0,   127,  10},     stream);
    print_ip(std::list<int>   {-1,  0,   127,  0xFF00}, stream);
    print_ip(std::vector<long>{-1,  127, 0xFF01},       stream);
    print_ip(std::list<long>  {0},                      stream);
    print_ip(std::vector<char>{},                       stream);
    

    BOOST_CHECK_EQUAL (stream.str(), "255.0.127.10\n"
                                     "255.0.127.0\n"
                                     "255.127.1\n"
                                     "0\n"
    );
}


BOOST_AUTO_TEST_SUITE_END()