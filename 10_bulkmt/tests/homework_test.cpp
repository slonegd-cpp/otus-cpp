#include "parser.h"
#include <sstream>
#include <fstream>


#define BOOST_TEST_MODULE homework_test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (simple_input_3_cmd)
{
    // input | output
    // ------|-------
    // cmd1  |
    // cmd2  |
    // cmd3  |
    //       | bulk: cmd1, cmd2, cmd3
    // cmd4  |
    // cmd5  |
    //       | bulk: cmd4, cmd5
    std::stringstream in;
    Parser parser (3, in);
    auto ss = std::make_shared<StringExporter>();
    parser.subscribe (ss);

    in << "cmd1\n"
       << "cmd2\n"
       << "cmd3\n"
       << "cmd4\n"
       << "cmd5\n";

    parser.parse();
    parser.endOfFile();

    BOOST_CHECK_EQUAL (ss->str(), 
        "bulk: cmd1, cmd2, cmd3\n"
        "bulk: cmd4, cmd5\n"
    );

}


BOOST_AUTO_TEST_CASE (input_with_brackets_3_cmd)
{
    // input | output
    // ------|-------
    // cmd1  |
    // cmd2  |
    // cmd3  |
    //       | bulk: cmd1, cmd2, cmd3
    // {     |
    // cmd4  |
    // cmd5  |
    // cmd6  |
    // cmd7  |
    // }     |
    //       | bulk: cmd4, cmd5, cmd6, cmd7
    std::stringstream in;
    Parser parser (3, in);
    auto ss = std::make_shared<StringExporter>();
    parser.subscribe (ss);

    in << "cmd1\n"
       << "cmd2\n"
       << "cmd3\n"
       << "{\n"
       << "cmd4\n"
       << "cmd5\n"
       << "cmd6\n"
       << "cmd7\n"
       << "}\n";

    parser.parse();
    parser.endOfFile();

    BOOST_CHECK_EQUAL (ss->str(), 
        "bulk: cmd1, cmd2, cmd3\n"
        "bulk: cmd4, cmd5, cmd6, cmd7\n"
    );
}


BOOST_AUTO_TEST_CASE (inner_brackets_ignore)
{
    // input | output
    // ------|-------
    // {     |
    // cmd1  |
    // cmd2  |
    // {     |
    // cmd3  |
    // cmd4  |
    // }     |
    // cmd5  |
    // cmd6  |
    // }     |
    //       | bulk: cmd1, cmd2, cmd3, cmd4, cmd5, cmd6
    std::stringstream in;
    auto ss = std::make_shared<StringExporter>();
    Parser parser (3, in);
    parser.subscribe (ss);

    in << "{\n"
       << "cmd1\n"
       << "cmd2\n"
       << "{\n"
       << "cmd3\n"
       << "cmd4\n"
       << "}\n"
       << "cmd5\n"
       << "cmd6\n"
       << "}\n";

    parser.parse();
    parser.endOfFile();

    BOOST_CHECK_EQUAL (ss->str(), 
        "bulk: cmd1, cmd2, cmd3, cmd4, cmd5, cmd6\n"
    );
}


BOOST_AUTO_TEST_CASE (end_of_file_in_brackets_ignore)
{
    // input | output
    // ------|-------
    // cmd1  |
    // cmd2  |
    // cmd3  |
    //       | bulk: cmd1, cmd2, cmd3
    // {     |
    // cmd4  |
    // cmd5  |
    // cmd6  |
    // cmd7  |
    std::stringstream in;
    Parser parser (3, in);
    auto ss = std::make_shared<StringExporter>();
    parser.subscribe (ss);

    in << "cmd1\n"
       << "cmd2\n"
       << "cmd3\n"
       << "{\n"
       << "cmd4\n"
       << "cmd5\n"
       << "cmd6\n"
       << "cmd7\n";

    parser.parse();
    parser.endOfFile();

    BOOST_CHECK_EQUAL (ss->str(), 
        "bulk: cmd1, cmd2, cmd3\n"
    );
}

BOOST_AUTO_TEST_SUITE_END()

