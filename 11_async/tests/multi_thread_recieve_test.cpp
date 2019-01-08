#include "async_wrapper.h"
#include <sstream>


#define BOOST_TEST_MODULE multi_thread_recieve_test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (multi_thread_recieve)
{
    std::stringstream out;
    std::cout.rdbuf(out.rdbuf());

    AsyncWrapper async {2};

    async.receive("cmd1\ncmd");

    std::thread t1 ([&](){ async.receive("2"); });
    t1.join();

    std::thread t2 ([&](){ async.receive("\ncmd3\n"); });
    t2.join();

    async.join();
    
    BOOST_CHECK_EQUAL (out.str(),
        "bulk: cmd1, cmd2\n"
        "bulk: cmd3\n"
    );

    std::system("rm bulk*.log");

}

BOOST_AUTO_TEST_SUITE_END()

