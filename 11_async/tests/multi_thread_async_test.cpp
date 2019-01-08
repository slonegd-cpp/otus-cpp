#include "async_wrapper.h"
#include <sstream>


#define BOOST_TEST_MODULE multi_thread_async_test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (multi_thread)
{
    std::stringstream in;
    AsyncWrapper async {2};
    std::stringstream out;
    std::cout.rdbuf(out.rdbuf());
    
    for (auto i {0}; i < 1000; ++i)
        async.receive ( "cmd" + std::to_string(i) +"\n" );

    async.join();

    auto get = [](const auto& m){
        std::size_t res {0};
        for (const auto& p : m)
            res += p.second;
        return res;
    };
    auto blocksQty   = get(async.blocksQty());
    auto commandsQty = get(async.cmdQty());
    auto threadQty   = async.threadQty();

    BOOST_CHECK_EQUAL (blocksQty,   500);
    BOOST_CHECK_EQUAL (commandsQty, 1000);
    BOOST_CHECK_EQUAL (threadQty,   2);

    std::system("rm bulk*.log");

}

BOOST_AUTO_TEST_SUITE_END()

