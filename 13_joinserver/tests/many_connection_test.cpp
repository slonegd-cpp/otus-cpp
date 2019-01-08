#include "server.h"
#include <chrono>
#include <thread>
#include <fstream>


#define BOOST_TEST_MODULE many_connection_test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (test)
{
   boost::asio::io_service service;
   boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 9999);

   Server server (service, endpoint);

   constexpr std::size_t connectionQty {10};

   std::fstream command[connectionQty];
   std::string answer[connectionQty];
   for (std::size_t i {0}; i < connectionQty; ++i) {
      command[i].open (std::string {"command" + std::to_string(i)}, std::ios_base::app);
      command[i] << "INSERT A " << std::to_string(i*10+0) << " lean\n"      ; answer[i] += "OK\n";
      command[i] << "INSERT A " << std::to_string(i*10+1) << " sweater\n"   ; answer[i] += "OK\n";
      command[i] << "INSERT A " << std::to_string(i*10+2) << " frank\n"     ; answer[i] += "OK\n";
      command[i] << "INSERT A " << std::to_string(i*10+3) << " violation\n" ; answer[i] += "OK\n";
      command[i] << "INSERT A " << std::to_string(i*10+4) << " quality\n"   ; answer[i] += "OK\n";
      command[i] << "INSERT A " << std::to_string(i*10+5) << " precision\n" ; answer[i] += "OK\n";

      command[i] << "INSERT B " << std::to_string(i*10+3) << " proposal\n"  ; answer[i] += "OK\n";
      command[i] << "INSERT B " << std::to_string(i*10+4) << " example\n"   ; answer[i] += "OK\n";
      command[i] << "INSERT B " << std::to_string(i*10+5) << " lake\n"      ; answer[i] += "OK\n";
      command[i] << "INSERT B " << std::to_string(i*10+6) << " flour\n"     ; answer[i] += "OK\n";
      command[i] << "INSERT B " << std::to_string(i*10+7) << " wonder\n"    ; answer[i] += "OK\n";
      command[i] << "INSERT B " << std::to_string(i*10+8) << " selection\n" ; answer[i] += "OK\n";

      command[i].close();
   }


   

   std::thread t( [&](){
      service.run();
   });

   using namespace std::chrono_literals;
   std::this_thread::sleep_for(100ms);
   std::string bash;
   for (std::size_t i {0}; i < connectionQty; ++i)
      bash += "cat command"
           + std::to_string(i) 
           + " | nc localhost 9999 > answer"
           + std::to_string(i)
           + " &";
   std::system(bash.c_str());
   std::this_thread::sleep_for(4s);


   std::fstream file[connectionQty];
   std::string file_answer[connectionQty];
   for (std::size_t i {0}; i < connectionQty; ++i) {
      file[i].open (std::string {"answer" + std::to_string(i)}, std::ios_base::in);
      std::string line;
      while (std::getline(file[i], line))
         file_answer[i] += line + '\n';
      file[i].close();

      BOOST_CHECK_EQUAL (file_answer[i], answer[i]);
   }


   service.stop();
   if (t.joinable())
      t.join();

   std::system("rm command*");
   std::system("rm answer*");
}

BOOST_AUTO_TEST_SUITE_END()
