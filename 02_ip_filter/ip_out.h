/**
 * выводит в поток ip, если выполняется условие
 */ 
#pragma once

#include <ostream>
#include <vector>

std::ostream& operator<< (std::ostream& stream, const std::vector<int>& ip)
{
    bool notFirst = false;
    for (auto& ip_part : ip) {
        if (notFirst)
            stream << ".";
        notFirst = true;
        stream << ip_part;
    }
    stream << std::endl;

    return stream;
}

std::ostream& operator<< (std::ostream& stream, const std::vector<std::vector<int>> ip)
{
    bool notFirst = false;
    for (auto& ip_part : ip) {
        if (notFirst)
            stream << ".";
        notFirst = true;
        stream << ip_part;
    }
    stream << std::endl;

    return stream;
}



