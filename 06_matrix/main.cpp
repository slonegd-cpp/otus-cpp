#include "homework.h"

int main(int argc, char const *argv[])
{
    try {

        process();

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
