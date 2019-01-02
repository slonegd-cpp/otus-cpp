#include "matrix.h"
#include <sstream>
#include <iostream>


#define BOOST_TEST_MODULE types_test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (types_test)
{
    const int freeElement = -1;
    Matrix<float, freeElement> matrix_float;
    Matrix<double, freeElement> matrix_double;
    float f = 0.1;
    double d = 0.01;
    matrix_float[1][2] = f;
    matrix_double[1][2] = 0.01;

    BOOST_REQUIRE_EQUAL (matrix_float[1][2], f);
    BOOST_REQUIRE_EQUAL (matrix_float[1][1], -1);
    BOOST_REQUIRE_EQUAL (matrix_double[1][2], d);
    BOOST_REQUIRE_EQUAL (matrix_double[1][1], -1);
}

BOOST_AUTO_TEST_SUITE_END()