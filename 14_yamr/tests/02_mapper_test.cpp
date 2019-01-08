#include "functions.h"

#define BOOST_TEST_MODULE 02_mapper_test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (test)
{

   auto result1 = mapper ("aother");
   BOOST_CHECK_EQUAL (result1.first, "a");
   BOOST_CHECK_EQUAL (result1.second, "other");

   auto result2 = mapper ("b");
   BOOST_CHECK_EQUAL (result2.first, "b");
   BOOST_CHECK_EQUAL (result2.second, "");

   // auto result3 = mapper (""); // результат не определён

   auto result4 = mapper ("cot\nher");
   BOOST_CHECK_EQUAL (result4.first, "c");
   BOOST_CHECK_EQUAL (result4.second, "ot\nher");  

}

BOOST_AUTO_TEST_SUITE_END()
