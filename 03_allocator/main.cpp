/**
 * ТЗ
 * 
 *  создание экземпляра std::map
 *  заполнение 10 элементами, где ключ это число от 0 до 9, а значение - факториал ключа
 *  создание экземпляра std::map с новым аллокатором ограниченным 10 елементами
 *  заполнение 10 элементами, где ключ это число от 0 до 9, а значение - факториал ключа
 *  вывод на экран всех значений (ключ и значение разделены пробелом)хранящихся в контейнере
 *  создание экземпляра своего контейнера для хранения int
 *  заполнение 10 элементами от 0 до 9
 *  создание экземпляра своего контейнера для хранения int с новым аллокатором
 *      ограниченным 10 елементами
 *  заполнение 10 элементами от 0 до 9
 *  вывод на экран всех значений хранящихся в контейнере
 * 
 * с. (-) Избегайте дублирования кода.
 * Напрашивается constexpr-функция для вычисления факториала.
 * Помните, что преждевременная оптимизация может повредить.
 * e. (-) Напрашивается использование std::generate_n, std::inserter.
 * Ясность решений - это не простота конструкций,
 * а использование таких стандартных конструкций,
 * которые явно показывают ваши намерения. 
 * 
 * a.(+-) Тесты все же явно не показывают выполнения требований задания.
 * В требованиях говорится о работе с map!
 * i.(!) https://goo.gl/rDfNXp - это усложнение.
 * Достаточно взять значение переменной my::alloc_counter до работы с контейнером
 * и после (но до уничтожения контейнера).
 * j.(-) Почему вместо char [sizeof(T)] - не использовать std::array
 * (какое-то смешение старых и новых подходов)
 * k.(!) Опционально: реализуйте полноценный аллокатор.
 * Это позволит вам в будущем ссылаться на вашу реализацию, чтобы показать,
 * что вы умеете работать с памятью!
 * k.(-) Желательно уйти от голых указателей!
 */
#include <iostream>
#include <map>
#include "allocator.h"
#include "list.h"
#include <list>
#include<algorithm> 

constexpr auto factorial (auto n) -> decltype(n)
{
    return n > 0 ? n * factorial(n - 1) : 1;
}

const size_t reserve = 10;
const size_t qty = 12;

int main(int argc, char const *argv[])
{
    try {

        auto make_pair_i_factorial_i = [i=0] () mutable
        {
            auto f = factorial(i);
            auto value = std::make_pair(i,f);
            ++i;
            return value;
        };

        // std::map c аллокатором по умолчанию
        std::map<int, int> m1;
        std::generate_n (
            std::inserter(m1, m1.begin()),
            qty,
            make_pair_i_factorial_i
        );

        // std::map с моим аллокатором 
        std::map<int, int, std::less<int>, MyAllocator<std::pair<const int, int> , reserve>> m2;
        std::generate_n (
            std::inserter(m2, m2.begin()),
            qty,
            make_pair_i_factorial_i
        );

        // вывод std::map с моим аллокатором
        for (auto p : m2)
            std::cout << p.first << ' ' << p.second << std::endl;

        // мой контейнер с аллокатором по умолчанию
        list<int> l1;
        for (int i = 0; i < qty; ++i)
            l1.push(i);

        // мой контейнер с моим аллокатором
        list<int, MyAllocator<int, reserve>> l2;
        for (int i = 0; i < qty; ++i)
            l2.push(i);


        // вывод моего контейнера с моим аллокатором
        for (auto n : l2)
            std::cout << n << std::endl;

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }


    return 0;
}
