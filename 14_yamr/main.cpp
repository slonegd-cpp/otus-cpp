#include <stdexcept>
#include "functions.h"


const std::string error =
   "Usage: yamr <src> <mnum> <rnum>\n"
   "<src>     file name\n"
   "<mnum>    1-99\n"
   "<rnum>    1-99\n";

auto parseArg (std::string str)
{
   if ( not std::all_of (std::cbegin(str), std::cend(str), [](const auto& c) { return std::isdigit(c); }) )
      throw std::invalid_argument (error);  
   size_t res;
   try {
      res = std::stoi(str);
   } catch (const std::exception &e) {
      throw std::invalid_argument (error);
   }
   if (res < 1 or res > 99)
      throw std::out_of_range (error);
   return res;
}


int main(int argc, char* argv[])
{
   try {
      if (argc < 4)
         throw std::invalid_argument (error);
      std::string filename {argv[1]};
      auto mnum = parseArg({argv[2]});
      auto rnum = parseArg({argv[3]});

      map_reduce (filename, mnum, rnum, mapper, reducer);

   } catch (std::exception& e) {
      std::cerr << e.what() << std::endl;
   }

   return 0;
}