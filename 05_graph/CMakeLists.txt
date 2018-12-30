cmake_minimum_required (VERSION 3.2)

# задать название и версию пакета
# условие, чтобы собирался дома
if ($ENV{TRAVIS_BUILD_NUMBER})
    project (graph VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project (graph VERSION 0.0.1)
endif()

find_package (Boost COMPONENTS unit_test_framework REQUIRED)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ../build)

# Создает исполняемый файл из исходников
add_executable (graph main.cpp)

# Задаёт опции компиляции
set_target_properties (graph PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)

enable_testing()

add_subdirectory(tests)