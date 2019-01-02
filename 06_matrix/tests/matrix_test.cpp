#include "matrix.h"
#include <sstream>


#define BOOST_TEST_MODULE matrix_test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (create_empty_matrix)
{
    const int freeElement = -1;
    Matrix<int, freeElement> matrix;
    BOOST_REQUIRE_EQUAL (matrix.size(), 0);
}

BOOST_AUTO_TEST_CASE (empty_element_check)
{
    const int freeElement = -1;
    Matrix<int, freeElement> matrix;
    auto a = matrix[0][0];
    BOOST_REQUIRE_EQUAL (a, freeElement);
    BOOST_REQUIRE_EQUAL (matrix.size(), 0);
}

BOOST_AUTO_TEST_CASE (add_element_to_matrix)
{
    const int freeElement = -1;
    Matrix<int, freeElement> matrix;
    matrix[100][100] = 314;
    BOOST_REQUIRE_EQUAL (matrix[100][100], 314);
    BOOST_REQUIRE_EQUAL (matrix.size(), 1);
}

BOOST_AUTO_TEST_CASE (assignment_test)
{
    const int freeElement = -1;
    Matrix<int, freeElement> matrix;
    ((matrix[1][100] = 314) = freeElement) = 217;
    BOOST_REQUIRE_EQUAL (matrix[1][100], 217);
    BOOST_REQUIRE_EQUAL (matrix.size(), 1);
}

BOOST_AUTO_TEST_CASE (out_with_position)
{
    const int freeElement = -1;
    Matrix<int, freeElement> matrix;
    std::stringstream stream;
    matrix[100][100] = 314;
    matrix[1][100] = 217;
    matrix[3][3] = 42;
    matrix.sort( [](const auto &lhs, const auto &rhs) {
        auto lx1 = std::get<0>(lhs);
        auto lx2 = std::get<1>(lhs);
        auto rx1 = std::get<0>(rhs);
        auto rx2 = std::get<1>(rhs);
        return lx1 < rx1 or lx1 == rx1 and lx2 < rx2;
    });
    for(const auto& c: matrix) {
        int x;
        int y;
        int v;
        std::tie(x, y, v) = c;
        stream << '[' << x << "][" << y << "]=" << v << std::endl;
    }
    BOOST_REQUIRE_EQUAL (stream.str(),
        "[1][100]=217\n"
        "[3][3]=42\n"
        "[100][100]=314\n"
    );
}

BOOST_AUTO_TEST_CASE (delete_element_check)
{
    const int freeElement = -1;
    Matrix<int, freeElement> matrix;
    std::stringstream stream;
    matrix[100][100] = 314;
    matrix[1][100] = 217;
    matrix[3][3] = 42;
    BOOST_REQUIRE_EQUAL (matrix.size(), 3);
    matrix[1][100] = freeElement;
    BOOST_REQUIRE_EQUAL (matrix.size(), 2);
}

BOOST_AUTO_TEST_CASE (delete_empty_element_check)
{
    const int freeElement = -1;
    Matrix<int, freeElement> matrix;
    std::stringstream stream;
    matrix[100][100] = 314;
    matrix[1][100] = 217;
    matrix[3][3] = 42;
    matrix[10][10] = freeElement;
    BOOST_REQUIRE_EQUAL (matrix.size(), 3);
}

BOOST_AUTO_TEST_CASE (matrix_3_check)
{
    ///те же тесты, только с трёхмерной матрицей
    const int freeElement = -1;
    const int matrixSize = 3;
    Matrix<int, freeElement, matrixSize> matrix;
    BOOST_REQUIRE_EQUAL (matrix.size(), 0);
    auto a = matrix[0][0][0];
    BOOST_REQUIRE_EQUAL (a, freeElement);
    BOOST_REQUIRE_EQUAL (matrix.size(), 0);
    matrix[100][100][100] = 314;
    BOOST_REQUIRE_EQUAL (matrix[100][100][100], 314);
    BOOST_REQUIRE_EQUAL (matrix.size(), 1);
    ((matrix[1][100][10] = 314) = freeElement) = 217;
    BOOST_REQUIRE_EQUAL (matrix[1][100][10], 217);
    BOOST_REQUIRE_EQUAL (matrix.size(), 2);
    matrix[3][3][1] = 42;
    matrix.sort( [](const auto &lhs, const auto &rhs) {
        auto lx1 = std::get<0>(lhs);
        auto lx2 = std::get<1>(lhs);
        auto lx3 = std::get<2>(lhs);
        auto rx1 = std::get<0>(rhs);
        auto rx2 = std::get<1>(rhs);
        auto rx3 = std::get<2>(rhs);
        return lx1 < rx1 
            or lx1 == rx1 and lx2 < rx2
            or lx1 == rx1 and lx2 == rx2 and lx3 < rx3;
    });
    std::stringstream stream;
    for(const auto& c: matrix) {
        int x,y,z,v;
        std::tie(x, y, z, v) = c;
        stream << '[' << x << "][" << y << "][" << z << "]=" << v << std::endl;
    }
    BOOST_REQUIRE_EQUAL (stream.str(),
        "[1][100][10]=217\n"
        "[3][3][1]=42\n"
        "[100][100][100]=314\n"
    );
    BOOST_REQUIRE_EQUAL (matrix.size(), 3);
    matrix[1][100][10] = freeElement;
    BOOST_REQUIRE_EQUAL (matrix.size(), 2);
    matrix[10][10][1] = freeElement;
    BOOST_REQUIRE_EQUAL (matrix.size(), 2);
}

BOOST_AUTO_TEST_SUITE_END()