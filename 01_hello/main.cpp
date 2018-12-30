#include <iostream>
#include "version.h"

int main (int argc, char* argv[])
{
    std::cout << "Hello, World!" << std::endl;
    std::cout << "Version: " << version() << std::endl;
    return 0;
}