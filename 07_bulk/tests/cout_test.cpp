#include "parser.h"
#include <sstream>
#include <fstream>


#define BOOST_TEST_MODULE cout_test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (simple_input_3_cmd)
{
    std::stringstream in, out;
    std::cout.rdbuf(out.rdbuf());
    Parser parser (3, in);
    auto ptr = parser.create_shared_and_subscribe<ConsoleExporter>();

    in << "cmd1\n"
       << "cmd2\n"
       << "cmd3\n"
       << "cmd4\n"
       << "cmd5\n";

    parser();

    BOOST_CHECK_EQUAL (out.str(), 
        "bulk: cmd1, cmd2, cmd3\n"
        "bulk: cmd4, cmd5\n"
    );
}



BOOST_AUTO_TEST_SUITE_END()

