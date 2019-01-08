#include "parser.h"
#include "threadsubs.h"
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <chrono>
#include <thread>


#define BOOST_TEST_MODULE benchmark_test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_suite_benchmark)


auto benchmark(std::size_t threadQty, std::size_t loadCPU)
{
   std::stringstream in;
   Parser parser (10, in);
   auto file = parser.create_shared_and_subscribe<ThreadFileExporter>(threadQty);

   file->loadCPU = loadCPU;

   for (auto i {0}; i < 1000; ++i)
      in << "cmd" << i << '\n';

   auto start = std::chrono::high_resolution_clock::now();
   parser.parse();
   parser.endOfFile();
   file->pool->join();
   auto end = std::chrono::high_resolution_clock::now();

   std::system("find . -name \"bulk*.log\" -delete");

   return std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
}

#define BENCHMARK(qty,load) BOOST_TEST_MESSAGE(std::to_string(qty) << " file thread, загрузка " << std::to_string(load) << ": " << benchmark(qty,load) << "ms")


BOOST_AUTO_TEST_CASE(file_thread)
{

   boost::unit_test::unit_test_log_t::instance().set_threshold_level( boost::unit_test::log_messages );

   BOOST_TEST_MESSAGE("Benchmark:");
   BENCHMARK(1, 5000);
   BENCHMARK(2, 5000);
   BENCHMARK(3, 5000);
   BENCHMARK(4, 5000);
   std::cout << std::endl;
   BENCHMARK(1, 10000);
   BENCHMARK(2, 10000);
   BENCHMARK(3, 10000);
   BENCHMARK(4, 10000);
   std::cout << std::endl;
   BENCHMARK(1, 20000);
   BENCHMARK(2, 20000);
   BENCHMARK(3, 20000);
   BENCHMARK(4, 20000);
   BENCHMARK(5, 20000);
   BENCHMARK(6, 20000);
   BENCHMARK(7, 20000);
   BENCHMARK(8, 20000);

   boost::unit_test::unit_test_log_t::instance().set_threshold_level( boost::unit_test::log_all_errors );

   
}

BOOST_AUTO_TEST_SUITE_END()

