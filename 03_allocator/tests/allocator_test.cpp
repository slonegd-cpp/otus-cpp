/**
 *  тесты по заданию
 *  Реализовать  свой  аллокатор  памяти,  который  позволит  выполнять
 *  операцию reserve для контейнера std::map. Аллокатор должен параметризоваться
 *  количеством выделяемых за раз элементов.  Освобождение конкретного
 *  элемента не предполагается - аллокатор должен освобождать всю память
 *  самостоятельно.  Дальнейшее расширение занятой памяти на усмотрение автора.
 *  Достаточно иметь фиксированный размер.
 * 
 *  Минимальные требования к аллокатору:
 *  1. Выделить необходимое фиксированое количество памяти.
 *  2. Деаллоцировать не должен.
 *  3. Расширение памяти сверх фиксированого не предусматривается.
 * 
 * a. (+-) Тестов маловато. Первое на, что я смотрю - это на тесты.
 * Если по представленным тестам могу ответить на вопрос:
 * задание выполнено верно? - то тесты хорошие.
 * Если ответа на такой вопрос найти не смог или
 * было сложно однозначно ответить на него - то тесты сомнительны.
 * Вот, как у вас сейчас. Всё, что вы тестируете в тестах,
 * для самих тестов должно быть “черным ящиком”.
 * Т.е. проверять BOOST_CHECK ((p[0] + 2) == p[2]); - это проверять реализацию,
 * т.е. внутреннее строение аллокатора, что неверно.
 * Если бы было специальное требование, что выдача памяти должна быть последовательной,
 * тогда этот тест еще мог иметь право на жизнь.
 * Когда создается специальный функционал на выделение памяти,
 * то желательно иметь тесты на утечки памяти.
 * Вспомните пример, который мы обсуждали на вебинаре https://goo.gl/PmMMwF.
 * Можно воспользоваться специальными функциями my::malloc и my::free,
 * чтобы определить в тесте всю ли память вы отдали!
 * 
 */
#include "newdelete.h"
#include "allocator.h"
#include <list>

#define BOOST_TEST_MODULE out_test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (test_suite_main)

// тест на уменьшение количества обращений на выделение памяти
// относительно стандартного
BOOST_AUTO_TEST_CASE (test_allocator_less_allocations)
{
    const std::size_t reserve {3};
    std::list<int> l_stdAlloc;
    std::list<int, MyAllocator<int, reserve>> l_myAlloc;

    auto allocateCount = [](auto& list) {
        std::size_t res{0};
        for (int i {0}; i < reserve; ++i) {
            std::size_t tmp {my::alloc_counter};
            list.push_back(1);
            res += my::alloc_counter - tmp;
        }
        return res;
    };

    BOOST_CHECK_EQUAL (allocateCount(l_myAlloc), 1);
    BOOST_CHECK_EQUAL (allocateCount(l_stdAlloc), reserve);
}

// // тест на количество запросов сверх зарезервированного
// BOOST_AUTO_TEST_CASE (test_allocator_incorrect)
// {

//     const size_t reserve = 3;
//     MyAllocator<int, reserve> alloc;
//     int* p = nullptr;
//     p = alloc.allocate(reserve);

//     BOOST_CHECK_THROW(alloc.allocate(1), std::exception);
// }

// // тест на то, что удаление не предусмотрено
// BOOST_AUTO_TEST_CASE (test_allocator_incorrect_deallocate)
// {

//     const size_t reserve = 3;
//     MyAllocator<int, reserve> alloc;
//     int* p = nullptr;
//     p = alloc.allocate(1);
//     p = alloc.allocate(1);
//     p = alloc.allocate(1);
//     alloc.deallocate(p,3);

//     BOOST_CHECK_THROW(alloc.allocate(1), std::exception);
// }


BOOST_AUTO_TEST_SUITE_END()