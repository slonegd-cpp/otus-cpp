#pragma once

#include <algorithm>
#include <vector>

template<class ... Ts>
std::vector<std::vector<int>> filter (const std::vector<std::vector<int>>& in, Ts ... args)
{
    std::vector<std::vector<int>> out;
    std::vector<int> vectorIn {args...};
    int array[sizeof...(Ts)] {args...};
    auto filter_ =  [vectorIn] (const std::vector<int>& ip) {
       auto it = ip.begin();
       bool equal {true};
       for (auto octet : vectorIn) {
          equal &= (octet == *it++);
       }
       return equal;
    };
    std::copy_if (
        in.begin(),
        in.end(),
        std::back_inserter(out),
        filter_
    );

    return out;
}

std::vector<std::vector<int>> filter_any (const std::vector<std::vector<int>>& in, int arg)
{
    std::vector<std::vector<int>> out;
    auto filter_ =  [arg] (const std::vector<int>& ip) {
        return std::any_of (
            ip.begin(),
            ip.end(),
            [arg](const int& n) {
                return n == arg;
            }
        );
    };
    std::copy_if (
        in.begin(),
        in.end(),
        std::back_inserter(out),
        filter_
    );

    return out; 
}