#include "functions.h"
#include <fstream>

#define BOOST_TEST_MODULE 04_homework_test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (test)
{
   std::fstream test ("test", std::ios_base::app);
   test << "amail1@mail.ru\n"
        << "amail2@mail.ru\n"
        << "amail3@mail.ru\n"
        << "amail4@mail.ru\n"
        << "amail5@mail.ru\n"
        << "bmail01@mail.ru\n"
        << "bmail02@mail.ru\n"
        << "bmail03@mail.ru\n"
        << "bmail04@mail.ru\n"
        << "bmail05@mail.ru\n"
        << "cmail001@mail.ru\n"
        << "cmail002@mail.ru\n"
        << "cmail003@mail.ru\n"
        << "cmail004@mail.ru\n"
        << "cmail005@mail.ru\n";
   test.close();

   std::stringstream out1;
   auto mapperQty {2}, reducerQty {1};
   map_reduce ("test", mapperQty, reducerQty, mapper, reducer, out1);
   BOOST_CHECK_EQUAL (out1.str(), "reducer 0: \n8\n");

   std::stringstream out2;
   mapperQty = 3;
   map_reduce ("test", mapperQty, reducerQty, mapper, reducer, out2);
   BOOST_CHECK_EQUAL (out2.str(), "reducer 0: \n8\n");

   std::stringstream out3;
   reducerQty = 2;
   map_reduce ("test", mapperQty, reducerQty, mapper, reducer, out3);
   BOOST_CHECK (out3.str() == "reducer 0: \n"
                              "6\n"
                              "reducer 1: \n"
                              "8\n"
             or out3.str() == "reducer 1: \n"
                              "8\n"
                              "reducer 0: \n"
                              "6\n"
   );

   std::system("rm test");

}

BOOST_AUTO_TEST_SUITE_END()
