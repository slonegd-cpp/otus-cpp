#pragma once

#include <string>
#include <vector>
#include <map>
#include <thread>
#include <algorithm>
#include <iterator>
#include <mutex>
#include "split.h"


std::pair<std::string, std::string> mapper_ (std::string);
using Mapper = decltype(mapper_);

using MapIterator = std::multimap<std::string, std::string>::iterator;
struct Iterators {
   MapIterator begin;
   MapIterator end;
};
std::string reducer_ (const std::vector<Iterators>& iterators, std::size_t first, std::size_t last);
using Reducer = decltype(reducer_);


void map_reduce (
   const std::string& src,
   std::size_t mapperQty,
   std::size_t reducerQty,
   Mapper      mapper,
   Reducer     reducer,
   std::ostream& stream = std::cout
)
{
   auto positions = split (src,mapperQty);

   std::multimap<std::string, std::string> mapper_result;
   std::mutex mtx;

   auto map_worker = [&](std::size_t n)
   {
      std::fstream file (src, std::ios_base::in);
      file.seekg(positions[n].begin);
      while (file.tellg() < positions[n].end) {
         std::string line;
         std::getline (file, line);
         auto pair = mapper(line);
         mtx.lock();
         mapper_result.insert (pair);
         mtx.unlock();
      }
   };

   // запуск маперов
   std::vector<std::thread> mapperThreads;
   mapperThreads.reserve (mapperQty);
   for (auto i{0}; i < mapperQty; ++i)
      mapperThreads.emplace_back (map_worker, i);

   for (auto it = std::begin(mapperThreads); it != std::end(mapperThreads); ++it)
      if (it->joinable())
         it->join();

   // печать результатов маппера
   // for (auto it = std::begin(mapper_result); it != std::end(mapper_result); ++it)
   //    stream << it->first << ' ' << it->second << std::endl;


   // выделяем итераторы по ключам
   std::vector<Iterators> iterators;
   Iterators tmp {std::begin(mapper_result), std::begin(mapper_result)};
   auto it = std::begin(mapper_result);
   while (it != std::end(mapper_result)) {
      it = mapper_result.upper_bound(it->first);
      tmp.end = it;
      iterators.push_back (tmp);
      tmp.begin = tmp.end;
   };
   // количество ключей
   // stream << iterators.size() << std::endl;
 

   auto reduce_worker = [&](std::size_t n)
   {
      std::string str {"reducer " + std::to_string(n)};
      // разделение ключей по редьюсерам
      auto first = iterators.size() / reducerQty * n;
      auto last = n == (reducerQty - 1) ? iterators.size() : iterators.size() / reducerQty * (n+1);
      last--;
      auto answer = reducer (iterators, first, last);
      std::fstream file (str, std::ios_base::out);
      file << answer;
      str += ": \n" + answer;
      mtx.lock();
      stream << str << std::endl;
      mtx.unlock();
   };
      
   // запуск редьюсеров
   std::vector<std::thread> reducerThreads;
   reducerThreads.reserve (reducerQty);
   for (auto i{0}; i < reducerQty; ++i)
      reducerThreads.emplace_back (reduce_worker, i);


   for (auto it = std::begin(reducerThreads); it != std::end(reducerThreads); ++it)
      if (it->joinable())
         it->join();

}