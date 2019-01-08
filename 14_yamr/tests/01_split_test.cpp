#include "split.h"
#include <fstream>


#define BOOST_TEST_MODULE 01_split_test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (test)
{
   std::fstream file ("test", std::ios_base::app);

   for (auto i{0}; i < 100; ++i)
      file << "line" << i << '\n';

   file.close();

   auto test1 = split ("test", 3);
   auto test2 = split ("test", 7);

   BOOST_CHECK_EQUAL (test1.size(), 3);
   BOOST_CHECK_EQUAL (test2.size(), 7);

   file.open ("test", std::ios_base::in);

   for (auto t : test1) {
      std::cout << t.begin << ' ' << t.end << std::endl;
      file.seekg(t.begin);
      BOOST_CHECK_EQUAL (file.peek(), 'l');
      file.seekg(t.end-1);
      BOOST_CHECK_EQUAL (file.peek(), '\n');
   }

   for (auto t : test2) {
      std::cout << t.begin << ' ' << t.end << std::endl;
      file.seekg(t.begin);
      BOOST_CHECK_EQUAL (file.peek(), 'l');
      file.seekg(t.end-1);
      BOOST_CHECK_EQUAL (file.peek(), '\n');
   }


   std::system("rm test");
}

BOOST_AUTO_TEST_SUITE_END()
