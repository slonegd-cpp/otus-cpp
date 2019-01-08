#include "functions.h"

#define BOOST_TEST_MODULE 03_reducer_test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (test1)
{

   std::multimap<std::string,std::string> in;
   in.emplace ("a", "bcdefi");
   in.emplace ("a", "bcdegi");
   BOOST_CHECK_EQUAL (computeFirstNoDuplicate(in.begin(), in.end()), 6);
   in.emplace ("b", "bcddgi");
   BOOST_CHECK_EQUAL (computeFirstNoDuplicate(in.begin(), in.end()), 6);
   in.emplace ("результат не зависит от ключа", "bcdggi");
   BOOST_CHECK_EQUAL (computeFirstNoDuplicate(in.begin(), in.end()), 6);

   in.clear();
   in.emplace ("a", "b");
   in.emplace ("a", "c");
   BOOST_CHECK_EQUAL (computeFirstNoDuplicate(in.begin(), in.end()), 2);
   in.emplace ("результат не зависит от ключа", "d");
   BOOST_CHECK_EQUAL (computeFirstNoDuplicate(in.begin(), in.end()), 2);

   in.clear();
   in.emplace ("a", "");
   BOOST_CHECK_EQUAL (computeFirstNoDuplicate(in.begin(), in.end()), 1);

   in.clear();
   std::string str {"если только 2 одинаковых"};
   in.emplace ("a", str);
   in.emplace ("a", str);
   BOOST_CHECK_EQUAL (computeFirstNoDuplicate(in.begin(), in.end()), str.size() + 1);

}


BOOST_AUTO_TEST_CASE (test2)
{

   std::multimap<std::string,std::string> in;
   in.emplace ("a", "bdc");
   in.emplace ("a", "bcdf");

   std::vector<Iterators> iterators;
   iterators.push_back (Iterators {in.begin(), in.end()});

   BOOST_CHECK_EQUAL (reducer(iterators, 0, 0), "3");

   // итераторы выровнены по ключу
   in.emplace ("b", "bcde");
   in.emplace ("b", "bcfe");
   iterators.clear();
   auto it = in.begin();
   it++;it++;
   iterators.push_back (Iterators {in.begin(), it});
   iterators.push_back (Iterators {it, in.end()});
   BOOST_CHECK_EQUAL (reducer(iterators, 0, 0), "3");
   BOOST_CHECK_EQUAL (reducer(iterators, 1, 1), "4");
   // результат - максимальное из двух предыдущих
   BOOST_CHECK_EQUAL (reducer(iterators, 0, 1), "4");

   // итераторы не выровнены по ключу - неправильное использование
   iterators.clear();
   it = in.begin();
   it++;
   iterators.push_back (Iterators {in.begin(), it});
   iterators.push_back (Iterators {it, in.end()});
   BOOST_CHECK_EQUAL (reducer(iterators, 0, 0), "1");
   BOOST_CHECK_EQUAL (reducer(iterators, 1, 1), "5");
   BOOST_CHECK_EQUAL (reducer(iterators, 0, 1), "5");

}

BOOST_AUTO_TEST_SUITE_END()
