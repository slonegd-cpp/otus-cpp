#include "matrix.h"
#include <iostream>
#include <algorithm>

void process (std::ostream& stream = std::cout)
{
    const int freeElement = 0;
    Matrix<int, freeElement> matrix;

    // заполнение матрицы
    for (auto i = 0; i < 10; i++) {
        matrix[i][i] = i;
        matrix[i][9-i] = 9-i;
    }

    // вывод матрицы с [1,1] до [8,8]
    for (auto i = 1; i < 9; i++) {
        for (auto j = 1; j < 9; j++)
            stream << matrix[i][j] << ' ';
        stream << std::endl;
    }

    stream << "size: " << matrix.size() << std::endl;

    // только занятые ячейки
    for (const auto& t : matrix) {
        stream << "matrix[" << std::get<0>(t) 
                    << "][" << std::get<1>(t) 
                    << "]=" << std::get<2>(t)
                    << std::endl;
    }

}