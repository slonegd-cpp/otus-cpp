#include "parser.h"
#include "threadsubs.h"
#include <stdexcept>
#include <string>
#include <fstream>
	

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

      std::string str {argv[1]};
      if ( !std::all_of (std::cbegin(str), std::cend(str), [](const auto& c) {
            return std::isdigit(c);
         })
      )
         throw std::invalid_argument(error);

      size_t n {0};
      try {
         n = std::stoi(argv[1]);
      } catch (const std::exception &e) {
         throw std::invalid_argument (error);
      }
      if (n < 1 or n > maxCommandQty)
         throw std::out_of_range (error);

      Parser parser (n, std::cin);

      constexpr std::size_t threadQty {2};
      
      auto console = parser.create_shared_and_subscribe<ThreadConsoleExporter>();
      auto file    = parser.create_shared_and_subscribe<ThreadFileExporter>(threadQty);

      parser.parse();
      parser.endOfFile();

      file->pool->join();
      console->pool->join();

      std::cout << parser.metrics();
      std::cout << console->metrics();
      std::cout << file->metrics();

   } catch (const std::exception &e) {
      std::cerr << e.what() << std::endl;
   }

   return 0;
}
