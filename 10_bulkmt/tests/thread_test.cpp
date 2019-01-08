#include "parser.h"
#include "threadsubs.h"
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <chrono>
#include <thread>


#define BOOST_TEST_MODULE thread_test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (one_thread)
{
   std::stringstream in;
   
   Parser parser (3, in);
   auto file = parser.create_shared_and_subscribe<ThreadFileExporter>();

   in << "cmd1\n"
      << "cmd2\n"
      << "cmd3\n"
      << "cmd4\n"
      << "cmd5\n";

   parser.parse();
   parser.endOfFile();

   file->pool->join();

   auto blocksQty = std::cbegin(file->blocksQty)->second;
   auto commandsQty = std::cbegin(file->cmdQty)->second;

   BOOST_CHECK_EQUAL (blocksQty, 2);
   BOOST_CHECK_EQUAL (commandsQty, 5);

   std::system("rm bulk*.log");

}

BOOST_AUTO_TEST_CASE (many_thread)
{
   std::stringstream in;

   Parser parser (2, in);
   auto file = parser.create_shared_and_subscribe<ThreadFileExporter>(3);

   ThreadFileExporter::blocksQty.clear();
   ThreadFileExporter::cmdQty.clear();

   for (auto i {0}; i < 1000; ++i)
      in << "cmd" << i << '\n';

   parser.parse();
   parser.endOfFile();

   file->pool->join();

   auto get = [](const auto& m){
      std::size_t res {0};
      for (const auto& p : m)
         res += p.second;
      return res;
   };
   auto blocksQty = get(file->blocksQty);
   auto commandsQty = get(file->cmdQty);
   auto threadQty = file->blocksQty.size();

   BOOST_CHECK_EQUAL (blocksQty,   500);
   BOOST_CHECK_EQUAL (commandsQty, 1000);
   BOOST_CHECK_EQUAL (threadQty,   3);

   std::system("rm bulk*.log");

}


BOOST_AUTO_TEST_SUITE_END()

