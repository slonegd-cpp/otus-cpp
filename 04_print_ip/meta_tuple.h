/**
 * метафункции для работы с tuple
 */

#pragma once

#include <tuple>

/// метаструктура, проходящая в операторе() по элементам tuple
template<size_t index, class function, class ... T>
struct tupleForeach_ 
{
    void operator () (const std::tuple<T...>& t, function f)
    {
        const size_t i = sizeof...(T) - index;
        f(std::get<i>(t));
        tupleForeach_<index - 1, function, T...>() (t,f);
    }
};
/// для конца рекурсии
template<class function, class ... T>
struct tupleForeach_<0, function, T...>
{
    void operator () (const std::tuple<T...>& t, function f)
    {
    }
};



/// обёртка над структурой tupleForeach_ в виде функции
/// для дедукции параметров шаблона
/// с дополнительным параметром с какого элемента начинать
template<size_t from, class function, class ... T>
void tupleForeachFrom (const std::tuple<T...>& t, function f)
{
    static_assert (
        sizeof...(T) >= from,
        "недопустимый параметр from"
    );
    tupleForeach_<sizeof...(T) - from, function, T...>() (t,f);
}


/// метаструктура, определяющая, что все типы одинаковые
template<class ... Ts>
struct IsAllSame : std::true_type {};

/// сравнивает первые 2 типа и вызывает рекурсивно следующее сравнение 
template<class T, class U, class ... Ts>
struct IsAllSame<T,U,Ts...>
{
    static const bool value = std::is_same<T,U>::value and IsAllSame<U, Ts...>::value;
};

/// приведение к стандарту
template<class ... Ts>
constexpr bool is_all_same_v = IsAllSame<Ts...>::value;