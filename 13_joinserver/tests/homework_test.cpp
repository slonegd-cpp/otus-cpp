#include "server.h"
#include <chrono>
#include <thread>
#include <fstream>


#define BOOST_TEST_MODULE homework_test

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
   command << "INSERT A 1 sweater\n"   ; answer += "OK\n";
   command << "INSERT A 2 frank\n"     ; answer += "OK\n";
   command << "INSERT A 3 violation\n" ; answer += "OK\n";
   command << "INSERT A 4 quality\n"   ; answer += "OK\n";
   command << "INSERT A 5 precision\n" ; answer += "OK\n";

   command << "INSERT B 3 proposal\n"  ; answer += "OK\n";
   command << "INSERT B 4 example\n"   ; answer += "OK\n";
   command << "INSERT B 5 lake\n"      ; answer += "OK\n";
   command << "INSERT B 6 flour\n"     ; answer += "OK\n";
   command << "INSERT B 7 wonder\n"    ; answer += "OK\n";
   command << "INSERT B 8 selection\n" ; answer += "OK\n";

   command << "INTERSECTION\n"         ; answer += "3,violation,proposal\n";
                                         answer += "4,quality,example\n";
                                         answer += "5,precision,lake\n";
                                         answer += "OK\n";

   command << "SYMMETRIC_DIFFERENCE\n" ; answer += "0,lean,\n";
                                         answer += "1,sweater,\n";
                                         answer += "2,frank,\n";
                                         answer += "6,,flour\n";
                                         answer += "7,,wonder\n";
                                         answer += "8,,selection\n";
                                         answer += "OK\n";

   command << "TRUNCATE A\n"           ; answer += "OK\n";
   command << "INSERT A 0 lean\n"      ; answer += "OK\n"; // no duplicate error

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
