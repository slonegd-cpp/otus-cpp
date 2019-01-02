#include "matrix.h"
#include <sstream>
#include <iostream>


#define BOOST_TEST_MODULE bad_ptr_test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (bad_ptr_test)
{
    const int freeElement = -1;
    Matrix<int, freeElement> matrix;
    auto test = matrix[2][2];
    matrix.~Matrix();
    BOOST_CHECK_THROW (test = 0, std::bad_weak_ptr);
}

BOOST_AUTO_TEST_SUITE_END()