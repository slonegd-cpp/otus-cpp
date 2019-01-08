#include "parser.h"
#include <stdexcept>
#include <string>
	

enum class argException {qty, outOfRange};

constexpr size_t maxCommandQty = 255;

const std::string error =
   "Должен быть 1 параметр командной строки в диапазоне от 1 до "
   + std::to_string(maxCommandQty);


int main(int argc, char const *argv[])
{
   try {

      if (argc != 2)
         throw std::invalid_argument (error);

      size_t n {0};
      try {
         n = std::stoi(argv[1]);
      } catch (const std::exception &e) {
         throw std::invalid_argument (error);
      }
      if (n < 1 or n > maxCommandQty)
         throw std::out_of_range (error);

      Parser parser (n, std::cin);
      
      auto console = parser.create_shared_and_subscribe<ConsoleExporter>();
      auto file    = parser.create_shared_and_subscribe<FileExporter>();

      parser();

   } catch (const std::exception &e) {
      std::cerr << e.what() << std::endl;
   }

   return 0;
}
