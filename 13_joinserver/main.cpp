// #include <cstdlib>
// #include <deque>
// #include <iostream>
// #include <list>
// #include <memory>
// #include <set>
// #include <utility>
// #include <boost/asio.hpp>

#include <string>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include "server.h"




#include <chrono>
#include <condition_variable>
#include <iostream>
#include <queue>
#include <thread>


const std::string error =
   "Usage: join_server <port>\n"
   "<port>    0000-9999\n";




int main(int argc, char* argv[])
{
   try {
      if (argc < 2)
         throw std::invalid_argument (error);  
      std::string str {argv[1]};
      if ( not std::all_of (std::cbegin(str), std::cend(str), [](const auto& c) { return std::isdigit(c); }) )
         throw std::invalid_argument (error);  
      size_t port;
      try {
         port = std::stoi(str);
      } catch (const std::exception &e) {
         throw std::invalid_argument (error);
      }
      if (port < 0 or port > 9999)
         throw std::out_of_range (error);


      boost::asio::io_service service;
      boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);

      Server server (service, endpoint);

      service.run();

   } catch (std::exception& e) {
      std::cerr << e.what() << "\n";
   }

   return 0;
}