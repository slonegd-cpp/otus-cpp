#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include "meta_tuple.h"
#include <type_traits>


/// метафункция, определяет является ли тип контейнером
template <class T>
struct is_conteiner : std::false_type {};
/// специализация для вектора
template <class T>
struct is_conteiner<std::vector<T>> : std::true_type {};
/// специализация для листа
template <class T>
struct is_conteiner<std::list<T>> : std::true_type {};

template<class T>
constexpr bool is_conteiner_v = is_conteiner<T>::value;


/// специализация print_ip для целых чисел
template<class T>
std::enable_if_t<std::is_integral<T>::value, void>
print_ip(const T& arg, std::ostream& stream = std::cout, const std::string end = "\n")
{
    for (int i = 1; i < sizeof(T); ++i) {
        stream << (arg >> (sizeof(T) - i)*8 & 0xFF) << '.';
    }
    stream << (arg & 0xFF) << end;
}


/// специализация print_ip для строки
template<class T>
std::enable_if_t<std::is_same<T, std::string>::value, void>
print_ip(const T& arg, std::ostream& stream = std::cout, const std::string end = "\n")
{
    if (arg.size() != 0)
        stream << arg << end;
}


/// специализация print_ip для контейнеров целых чисел
template<class T>
std::enable_if_t<is_conteiner_v<T>, void>
print_ip(const T& arg, std::ostream& stream = std::cout, const std::string end = "\n")
{
    if (arg.size() != 0) {
        auto it = std::begin(arg);
        stream << (*it & 0xFF);
        for (auto it_ = std::next(it); it_ != std::end(arg); ++it_)
            stream << '.' << (*it_ & 0xFF);
        stream << end;
    }
}


/// специализация print_ip для tuple
template<class ... Types>
void print_ip(const std::tuple<Types...>& t, std::ostream& stream = std::cout, const std::string end = "\n")
{
    static_assert (
        sizeof...(Types) > 0,
        "нельзя пустой tuple"
    );
    static_assert (
        is_all_same_v<Types...>,
        "все типы должны быть одинаковые"
    );
    stream << (static_cast<char>(std::get<0>(t)) & 0xFF);

    tupleForeachFrom<1> (t, [&stream](auto n) { 
        stream << '.' << (static_cast<char>(n) & 0xFF);
    });

    stream << end;
}

