#pragma once

#include <vector>
#include <fstream>

#include <iostream>

struct Positions {
   std::size_t begin;
   std::size_t end;
};
std::vector<Positions> split (const std::string& filename, std::size_t qty)
{
   std::vector<Positions> res;
   std::fstream file (filename, std::ios_base::in);
   file.seekg (0, std::ios::end);
   auto sectorSize = file.tellg() / qty;
   file.seekg (0, std::ios::beg);

   Positions tmp {0,0};
   for (auto i{1}; i < qty; ++i) {
      tmp.end = sectorSize * i - 1;
      file.seekg (tmp.end);
      while (file.peek() != '\n') {
         tmp.end++;
         file.seekg (tmp.end);
      }
      tmp.end++;
      res.push_back (tmp);
      tmp.begin = tmp.end;
   }
   file.seekg (0, std::ios::end);
   tmp.end = file.tellg();
   res.push_back (tmp);
   
   return res;
}