#include "parser.h"
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <chrono>
#include <thread>


#define BOOST_TEST_MODULE file_test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (fast_simple_input_3_cmd)
{
    std::stringstream in, out;

    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    Parser parser (3, in);
    auto file = std::make_shared<FileExporter>();
    parser.subscribe (file);

    in << "cmd1\n"
       << "cmd2\n"
       << "cmd3\n"
       << "cmd4\n"
       << "cmd5\n";

    parser.parse();
    parser.endOfFile();

    file->open(file->name, std::ios_base::in);
    out << file->rdbuf();
    file->close();

    BOOST_CHECK_EQUAL (out.str(), 
        "bulk: cmd1, cmd2, cmd3\n"
        "bulk: cmd4, cmd5\n"
    );

}


BOOST_AUTO_TEST_CASE (slow_input)
{
    std::stringstream in, out[2];
    std::string name[2];
    time_t time;

    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    Parser parser (3, in);
    auto file = std::make_shared<FileExporter>();
    parser.subscribe (file);

    in << "cmd1\n"
       << "cmd2\n"
       << "cmd3\n";

    parser.parse();

    name[0] = file->name;

    std::this_thread::sleep_for(std::chrono::seconds(1));

    in.clear();
    in << "cmd4\n"
       << "cmd5\n"
       << "cmd6\n";

    parser.parse();
    parser.endOfFile();

    name[1] = file->name;

    file->open(name[0], std::ios_base::in);
    out[0] << file->rdbuf();
    file->close();
    file->open(name[1], std::ios_base::in);
    out[1] << file->rdbuf();
    file->close();

    BOOST_CHECK_EQUAL (out[0].str(), 
        "bulk: cmd1, cmd2, cmd3\n"
    );
    BOOST_CHECK_EQUAL (out[1].str(), 
        "bulk: cmd4, cmd5, cmd6\n"
    );
}



BOOST_AUTO_TEST_SUITE_END()

