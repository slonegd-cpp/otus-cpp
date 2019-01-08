#include "bulk_server.h"
#include <chrono>
#include <thread>


#define BOOST_TEST_MODULE bulk_server_test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (one_connector)
{
   boost::asio::io_service service;
   boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 9999);

   auto str = std::make_shared<StringExporter>();

   auto server = std::make_shared<BulkServer>(service, endpoint, 2)
      ->subscribe (str)
      ->start();

   std::fstream file ("test", std::ios_base::app);
   file << "1\n2\n3\n{\n4\n5\n6\n7\n}\n8\n9\n";
   file.close();

   std::thread t( [&](){
      service.run();
   });

   using namespace std::chrono_literals;
   std::this_thread::sleep_for(100ms);

   std::system("cat test | nc localhost 9999 &");

   std::this_thread::sleep_for(100ms);

   service.stop();
   t.join();

   BOOST_CHECK_EQUAL (str->str(),
      "bulk: 1, 2\n"
      "bulk: 4, 5, 6, 7\n"
      "bulk: 3, 8\n"
   );

   std::system("find . -name \"test\" -delete");
}

BOOST_AUTO_TEST_CASE (two_order_connectons)
{
   boost::asio::io_service service;
   boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 9999);

   auto str = std::make_shared<StringExporter>();

   auto server = std::make_shared<BulkServer>(service, endpoint, 2)
      ->subscribe (str)
      ->start();

   std::fstream file ("test", std::ios_base::app);
   file << "1\n2\n3\n{\n4\n5\n6\n7\n}\n8\n9\n";
   file.close();

   std::thread t( [&](){
      service.run();
   });

   using namespace std::chrono_literals;
   std::this_thread::sleep_for(100ms);
   std::system("cat test | nc localhost 9999 &");
   std::this_thread::sleep_for(100ms);
   std::system("cat test | nc localhost 9999 &");
   std::this_thread::sleep_for(100ms);

   service.stop();
   t.join();

   BOOST_CHECK_EQUAL (str->str(),
      "bulk: 1, 2\n"
      "bulk: 4, 5, 6, 7\n"
      "bulk: 3, 8\n"
      "bulk: 9, 1\n"
      "bulk: 2, 3\n"
      "bulk: 4, 5, 6, 7\n"
      "bulk: 8, 9\n"
   );

   std::system("find . -name \"test\" -delete");
}

BOOST_AUTO_TEST_CASE (two_parallel_connectons)
{
   boost::asio::io_service service;
   boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 9999);

   auto str = std::make_shared<StringExporter>();

   auto server = std::make_shared<BulkServer>(service, endpoint, 10)
      ->subscribe (str)
      ->start();

   std::fstream file ("test", std::ios_base::app);
   file << "1\n2\n3\n{\n4\n5\n6\n7\n}\n8\n9\n";
   file.close();

   std::thread t ([&](){
      service.run();
   });

   using namespace std::chrono_literals;
   std::this_thread::sleep_for(100ms);
   std::system("seq 0 100 | nc localhost 9999 & seq 0 100 | nc localhost 9999 &");
   std::this_thread::sleep_for(100ms);

   service.stop();
   t.join();

   // в выводе видно перемешивание
   std::cout << str->str();

   std::system("find . -name \"test\" -delete");
}


BOOST_AUTO_TEST_SUITE_END()
