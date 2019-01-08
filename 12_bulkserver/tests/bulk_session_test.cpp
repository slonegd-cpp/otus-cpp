#include "bulk_session.h"
#include <chrono>
#include <thread>


#define BOOST_TEST_MODULE bulk_session_test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (subscribe)
{
   boost::asio::io_service service;
   boost::asio::ip::tcp::socket socket(service);
   std::size_t bulk {2};
   Parser parser {bulk};

   auto session = std::make_shared<BulkSession>(std::move(socket), parser, bulk);

   std::vector<std::shared_ptr<Exporter>> subs;
   for (auto i {0}; i < 5; i++)
      subs.push_back (std::make_shared<ConsoleExporter>());


   BOOST_CHECK_EQUAL (session->subscribedQty(), 0);
   session->subscribe (subs);
   BOOST_CHECK_EQUAL (session->subscribedQty(), 5);
}



BOOST_AUTO_TEST_CASE (common_inner_command)
{
   boost::asio::io_service service;
   boost::asio::ip::tcp::socket socket(service);
   boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 9999);
   boost::asio::ip::tcp::acceptor acceptor(service, endpoint);

   std::size_t bulk {2};
   Parser parser {bulk};
   auto common = parser.create_shared_and_subscribe<StringExporter>();
   auto inner  = std::make_shared<StringExporter>();

   std::fstream file ("test", std::ios_base::app);
   file << "1\n2\n3\n{\n4\n5\n6\n7\n}\n8\n9\n";
   file.close();

   std::system("cat test | nc localhost 9999 &");

   acceptor.accept (socket);

   auto session = std::make_shared<BulkSession>(std::move(socket), parser, bulk)
      ->subscribe(inner)
      ->read_line();

   std::thread t2( [&](){
      service.run();
   });

   using namespace std::chrono_literals;
   std::this_thread::sleep_for(100ms);

   service.stop();
   t2.join();
  

   BOOST_CHECK_EQUAL (common->str(),
      "bulk: 1, 2\n"
      "bulk: 3, 8\n"
   );

   BOOST_CHECK_EQUAL (inner->str(),
      "bulk: 4, 5, 6, 7\n"
   );

   std::system("find . -name \"test\" -delete");
}

BOOST_AUTO_TEST_SUITE_END()
