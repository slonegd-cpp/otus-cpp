#include "version.h"
#include <sstream>

std::string version()
{
    std::stringstream s;
    s << PROJECT_VERSION_MAJOR << '.'
      << PROJECT_VERSION_MINOR << '.'
      << PROJECT_VERSION_PATCH;
    return s.str();
}