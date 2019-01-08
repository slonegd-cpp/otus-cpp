#include "parser.h"
#include "threadsubs.h"
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <chrono>
#include <thread>


#define BOOST_TEST_MODULE efficiency_test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_suite_benchmark)


template<std::size_t threadQty>
auto benchmark()
{
   std::stringstream in;
   Parser parser (10, in);
   auto file = parser.create_shared_and_subscribe<ThreadFileExporter>(threadQty);

   for (auto i {0}; i < 1000000; ++i)
      in << "cmd" << i << '\n';

   auto start = std::chrono::high_resolution_clock::now();
   parser.parse();
   parser.endOfFile();
   file->pool->join();
   auto end = std::chrono::high_resolution_clock::now();

   std::system("find . -name \"bulk*.log\" -delete");

   return std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
}

#define BENCHMARK(n)    auto ms##n = benchmark<n>(); \
   BOOST_TEST_MESSAGE(std::to_string(n) << " file thread: " << ms##n << "ms")


BOOST_AUTO_TEST_CASE(file_thread)
{

   boost::unit_test::unit_test_log_t::instance().set_threshold_level( boost::unit_test::log_messages );

   BOOST_TEST_MESSAGE("Benchmark 1 000 000 commands:");
   BENCHMARK(2);
   BENCHMARK(3);
   BENCHMARK(4);

   boost::unit_test::unit_test_log_t::instance().set_threshold_level( boost::unit_test::log_all_errors );

   
}

BOOST_AUTO_TEST_SUITE_END()

