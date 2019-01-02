#include "homework.h"
#include <sstream>


#define BOOST_TEST_MODULE homework_test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (homework_test)
{
    std::stringstream stream;
    process (stream);
    BOOST_CHECK (stream.str() == 
        "1 0 0 0 0 0 0 8 \n"
        "0 2 0 0 0 0 7 0 \n"
        "0 0 3 0 0 6 0 0 \n"
        "0 0 0 4 5 0 0 0 \n"
        "0 0 0 4 5 0 0 0 \n"
        "0 0 3 0 0 6 0 0 \n"
        "0 2 0 0 0 0 7 0 \n"
        "1 0 0 0 0 0 0 8 \n"
        "size: 18\n"
        "matrix[0][9]=9\n"
        "matrix[1][1]=1\n"
        "matrix[1][8]=8\n"
        "matrix[2][2]=2\n"
        "matrix[2][7]=7\n"
        "matrix[3][3]=3\n"
        "matrix[3][6]=6\n"
        "matrix[4][4]=4\n"
        "matrix[4][5]=5\n"
        "matrix[5][5]=5\n"
        "matrix[5][4]=4\n"
        "matrix[6][6]=6\n"
        "matrix[6][3]=3\n"
        "matrix[7][7]=7\n"
        "matrix[7][2]=2\n"
        "matrix[8][8]=8\n"
        "matrix[8][1]=1\n"
        "matrix[9][9]=9\n"
    );

}

BOOST_AUTO_TEST_SUITE_END()

