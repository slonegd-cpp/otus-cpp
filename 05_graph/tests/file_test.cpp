#define BOOST_TEST_MODULE file_test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (open_file_test)
{
    // открываем специально подготовленный файл
    // Document.data должен быть эквивалентен эталону
}

BOOST_AUTO_TEST_CASE (save_file_test)
{
    // сохраняем в файл подготовленный Document.data
    // сравниваем содержания файла с эталоном
}

BOOST_AUTO_TEST_SUITE_END()

