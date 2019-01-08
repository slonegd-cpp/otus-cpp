#include "parser.h"
#include "threadsubs.h"
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <chrono>
#include <thread>


#define BOOST_TEST_MODULE out_of_memory_test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (out_of_memory_test)

BOOST_AUTO_TEST_CASE (out_of_memory)
{
   std::stringstream in;
   
   Parser parser (1, in);
   constexpr bool out_of_memory_exception = true;
   auto file = parser.create_shared_and_subscribe<ThreadFileExporter>();

   file->failTest = true;

   in << "cmd1" << '\n'
      << "cmd2" << '\n'
      << "cmd3" << '\n';
   parser.parse();
   parser.endOfFile();

   BOOST_CHECK_THROW (file->pool->join(), std::runtime_error);
   
   std::system("rm bulk*.log");

}

BOOST_AUTO_TEST_SUITE_END()

