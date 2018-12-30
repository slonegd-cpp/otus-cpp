#define BOOST_TEST_MODULE document_test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (add_figure_test)
{
    // создаём пустой документ
    // добавляем несколько фигур
    // проверяем эквивалентность document.data
}

BOOST_AUTO_TEST_CASE (remove_figure_test)
{
    // создаём пустой документ
    // добавляем несколько фигур
    // проверяем эквивалентность document.data
    // удаляем одну фигуру
    // проверяем эквивалентность document.data
}

BOOST_AUTO_TEST_CASE (clear_test)
{
    // создаём пустой документ
    // добавляем несколько фигур
    // проверяем эквивалентность document.data
    // очищаем документ
    // document.data.size() == 0
}

BOOST_AUTO_TEST_SUITE_END()