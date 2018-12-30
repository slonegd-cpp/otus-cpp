#define BOOST_TEST_MODULE draw_test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (point_draw_test)
{
    // создаём пустой документ
    // добавляем несколько точек по разным координатом
    // отображаем на экране 
    // сравниваем отображение с эталоном
}

/**
 * аналогичные тесты для каждой из фигур
 */

BOOST_AUTO_TEST_CASE (remove_figure_draw_test)
{
    // создаём пустой документ
    // добавляем несколько фигур по разным координатам с перекрытием
    // отображаем на экране 
    // сравниваем отображение с эталоном
    // удаляем фигуру, перекрывающую другую
    // сравниваем отображение с эталоном
}

BOOST_AUTO_TEST_SUITE_END()

