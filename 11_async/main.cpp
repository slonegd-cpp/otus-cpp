#include "async_wrapper.h"


int main(int, char *[]) {
    try {
        constexpr std::size_t bulk {5};
        AsyncWrapper h  {bulk};
        AsyncWrapper h2 {bulk};
        h .receive ("1");
        h2.receive ("1\n");
        h .receive ("\n2\n3\n4\n5\n6\n{\na\n");
        h .receive ("b\nc\nd\n}\n89\n");
   } catch (const std::exception &e) {
      std::cerr << e.what() << std::endl;
   }


    return 0;
}
