#define BOOST_TEST_MODULE database_test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (save_and_find_document_test)
{
    // сохраняем подготовленный Document.data
    // ищем этот документ в базе 
    // не долно быть ошибок и полученный документ сравниваем с эталоном
}

BOOST_AUTO_TEST_CASE (find_dont_exist_document_test)
{
    // открываем не существующий документ
    // должна быть ошибка открытия
}

BOOST_AUTO_TEST_SUITE_END()

