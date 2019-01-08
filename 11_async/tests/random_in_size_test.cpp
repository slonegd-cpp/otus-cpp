#include "async_wrapper.h"
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <cmath>


#define BOOST_TEST_MODULE random_in_size_test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (random_in_size)
{
    std::stringstream out, res;
    std::string in;
    constexpr std::size_t bulk      {5};
    constexpr std::size_t cmdQty    {100000};
    constexpr std::size_t maxInSize {1000};
    AsyncWrapper async {bulk};
    std::cout.rdbuf(out.rdbuf());

    for (auto i {0}; i < cmdQty; ++i)
        in += "cmd" + std::to_string(i) + '\n';


    for (auto i {0}; i < cmdQty/bulk; ++i) {
        res << "bulk: cmd" << (i*bulk);
        for (auto j {1}; j < bulk; ++j)
            res << ", cmd" << (i*bulk + j);
        res << '\n';
    }

    std::srand(unsigned(std::time(0)));
    auto c_str = in.c_str();

    for (size_t i {0}; i < in.size();) {
        std::size_t r = std::rand() % maxInSize + 1;
        async.receive ( std::string {c_str + i, std::min(r, in.size() -i)} );
        i += r;
    }

    async.join();

    BOOST_CHECK_EQUAL (out.str(), res.str());

    std::system("find . -name \"bulk*.log\" -delete");

}



BOOST_AUTO_TEST_SUITE_END()

