#include "parser.h"
#include <sstream>
#include <fstream>


#define BOOST_TEST_MODULE exporter_test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (auto_subscribe)
{
   std::stringstream in;
   Parser parser (3, in);
   auto ss = parser.create_shared_and_subscribe<StringExporter>();

   in << "cmd1\n";

   parser();

   BOOST_CHECK_EQUAL (ss->str(),
      "bulk: cmd1\n"
   );
}

BOOST_AUTO_TEST_CASE (time_to_subscribe)
{
    std::stringstream in;
    Parser parser (3, in);
    auto ss1 = parser.create_shared_and_subscribe<StringExporter>();

    in << "cmd1\n"
       << "cmd2\n"
       << "cmd3\n"
       << "cmd4\n"
       << "cmd5\n";

    parser();

    auto ss2 = parser.create_shared_and_subscribe<StringExporter>();

    in.clear();

    in << "cmd6\n"
       << "cmd7\n";

    parser();

    BOOST_CHECK_EQUAL (ss1->str(), 
        "bulk: cmd1, cmd2, cmd3\n"
        "bulk: cmd4, cmd5\n"
        "bulk: cmd6, cmd7\n"
    );
    BOOST_CHECK_EQUAL (ss2->str(), 
        "bulk: cmd6, cmd7\n"
    );
}

BOOST_AUTO_TEST_CASE (delete_subscribe)
{
    std::stringstream in, out;
    std::cout.rdbuf(out.rdbuf());
    Parser parser (3, in);
    auto consolePtr = parser.create_shared_and_subscribe<ConsoleExporter>();
    auto ss         = parser.create_shared_and_subscribe<StringExporter>();

    in << "cmd1\n";

    parser();

    BOOST_CHECK_EQUAL (out.str(),
        "bulk: cmd1\n"
    );
    BOOST_CHECK_EQUAL (ss->str(),
        "bulk: cmd1\n"
    );

    consolePtr.reset();

    in.clear();
    in << "cmd2\n";

    parser();

    BOOST_CHECK_EQUAL (out.str(),
        "bulk: cmd1\n"
    );
    BOOST_CHECK_EQUAL (ss->str(),
        "bulk: cmd1\n"
        "bulk: cmd2\n"
    );
}

BOOST_AUTO_TEST_CASE (unsubscribe)
{
    std::stringstream in, out;
    std::cout.rdbuf(out.rdbuf());
    Parser parser (3, in);
    auto consolePtr = parser.create_shared_and_subscribe<ConsoleExporter>();
    auto ss         = parser.create_shared_and_subscribe<StringExporter>();

    in << "cmd1\n";

    parser();

    BOOST_CHECK_EQUAL (out.str(),
        "bulk: cmd1\n"
    );
    BOOST_CHECK_EQUAL (ss->str(),
        "bulk: cmd1\n"
    );

    parser.unsubscribe (consolePtr);

    in.clear();
    in << "cmd2\n";

    parser();

    BOOST_CHECK_EQUAL (out.str(),
        "bulk: cmd1\n"
    );
    BOOST_CHECK_EQUAL (ss->str(),
        "bulk: cmd1\n"
        "bulk: cmd2\n"
    );
}


BOOST_AUTO_TEST_SUITE_END()

