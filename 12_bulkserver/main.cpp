// #include <cstdlib>
// #include <deque>
// #include <iostream>
// #include <list>
// #include <memory>
// #include <set>
// #include <utility>
// #include <boost/asio.hpp>
#include "bulk_server.h"
#include "threadsubs.h"

const std::string error =
   "Usage: bulk_server <port> <bulk_size>\n"
   "<port>    0000-9999\n"
   "<bulk_size>  1-255\n";




int main(int argc, char* argv[])
{
   try {
      if (argc < 3) {
         std::cerr << error;
         return 1;
      }

      std::string str {argv[1]};
      if ( !std::all_of (std::cbegin(str), std::cend(str), [](const auto& c) { return std::isdigit(c); }) ) {
         throw std::invalid_argument (error);  
      }
      size_t port;
      try {
         port = std::stoi(str);
      } catch (const std::exception &e) {
         throw std::invalid_argument (error);
      }
      if (port < 0 or port > 9999)
         throw std::out_of_range (error);

      str = argv[2];
      if ( !std::all_of (std::cbegin(str), std::cend(str), [](const auto& c) { return std::isdigit(c); }) ) {
         throw std::invalid_argument (error);  
      }
      size_t bulk;
      try {
         bulk = std::stoi(str);
      } catch (const std::exception &e) {
         throw std::invalid_argument (error);
      }
      if (bulk < 1 or bulk > 255)
         throw std::out_of_range (error);



      boost::asio::io_service service;
      boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);

      auto console = std::make_shared<ConsoleExporter>();
      auto file    = std::make_shared<ThreadFileExporter>(2);

      auto server = std::make_shared<BulkServer>(service, endpoint, bulk)
         ->subscribe (console)
         ->subscribe (file)
         ->start();

      service.run();

   } catch (std::exception& e) {
      std::cerr << e.what() << "\n";
   }

   return 0;
}