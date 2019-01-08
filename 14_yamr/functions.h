#pragma once

#include <string>
#include <algorithm>
#include <iostream>
#include <set>
#include "map_reduce.h"



std::pair<std::string, std::string> mapper (std::string in)
{
   auto key = std::string {in.front()};
   auto value = in.erase (0,1);
   return std::make_pair (key, value);
}


auto computeFirstNoDuplicate (MapIterator begin, MapIterator end)
{
   std::set<std::string> set_;
   std::size_t index {0};
   do {
      for (auto it = begin; it != end; ++it) {
         if (it->second.size() > index) {
            std::string tmp = it->second.substr(0, index);
            if (set_.find(tmp) == std::cend(set_)) {
               set_.insert (tmp);
            } else {
               index++;
               set_.clear();
               break;
            }
         } else {
            set_.insert (it->second);
         }
      }
   } while (set_.empty());
   return ++index; // + key
}



std::string reducer (const std::vector<Iterators>& iterators, std::size_t first, std::size_t last)
{
   // std::string res;
   std::vector<std::size_t> indexses;
   // проход по всем ключам из диапазона
   for (auto i{first}; i <= last; ++i) {
      auto index = computeFirstNoDuplicate (iterators[i].begin, iterators[i].end);
      indexses.push_back(index);
      // промежуточные результаты
      // res += iterators[i].begin->first + ' ' + std::to_string(index) +'\n';
   }

   return std::to_string(*std::max_element(indexses.begin(), indexses.end()));
}

