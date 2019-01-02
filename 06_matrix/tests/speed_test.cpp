#include "matrix.h"
#include <sstream>
#include <iostream>
#include <chrono>


#define BOOST_TEST_MODULE speed_test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (speed_test)
{
    const int freeElement = -1;
    Matrix<float, freeElement> matrix;
    int z{0};

    auto start = std::chrono::high_resolution_clock::now();
    for(int i{}, j{10'000}; i < j; ++i, --j) {
        matrix[i][i] = i + ( ++z );
        matrix[i][j] = j + z;
    }
    auto end = std::chrono::high_resolution_clock::now();

    boost::unit_test::unit_test_log_t::instance().set_threshold_level( boost::unit_test::log_messages );
    BOOST_TEST_MESSAGE ("result: " << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << " ms" );
    boost::unit_test::unit_test_log_t::instance().set_threshold_level( boost::unit_test::log_all_errors );
}

BOOST_AUTO_TEST_SUITE_END()
