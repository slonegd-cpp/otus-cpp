#include "server.h"
#include <chrono>
#include <thread>
#include <fstream>


#define BOOST_TEST_MODULE error_test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (test)
{
   boost::asio::io_service service;
   boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 9999);

   Server server (service, endpoint);

   std::fstream command ("command", std::ios_base::app);
   std::string answer;
   command << "INSERT A 0 lean\n"      ; answer += "OK\n";
   command << "INSERT A 0 sweater\n"   ; answer += "ERR duplicate 0\n";
   command << "INSER A 0 sweater\n"    ; answer += "ERR Command Error\n";
   command << "INSERT C 0 sweater\n"   ; answer += "ERR Table Name Error\n";
   command << "INSERT A b sweater\n"   ; answer += "ERR ID Error\n";

   command.close();

   std::thread t( [&](){
      service.run();
   });

   using namespace std::chrono_literals;
   std::this_thread::sleep_for(100ms);
   std::system("cat command | nc localhost 9999 > answer");
   std::this_thread::sleep_for(1s);

   service.stop();
   if (t.joinable())
      t.join();

   std::fstream file ("answer", std::ios_base::in);
   std::string line, file_answer;
   while (std::getline(file, line))
      file_answer += line + '\n';


   BOOST_CHECK_EQUAL (file_answer, answer);


   std::system("rm command");
   std::system("rm answer");
}

BOOST_AUTO_TEST_SUITE_END()
