cmake_minimum_required(VERSION 3.2)
project(async)

if ($ENV{TRAVIS_BUILD_NUMBER})
    project (async VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project (async VERSION 0.0.1)
endif()

find_package (Threads)
find_package (Boost COMPONENTS unit_test_framework REQUIRED)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ../build)
set (CMAKE_BUILD_TYPE Debug)

add_library(async SHARED async.cpp async.h otus10/threadsubs.cpp)

add_executable(async_cli main.cpp)
add_executable(test4 test4.cpp)

set_target_properties (async async_cli test4 PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS -Wpedantic -Wall -Wextra
    COMPILE_FLAGS -O2
    INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/otus10
)

target_link_libraries (async_cli async)
target_link_libraries (test4     async)

target_link_libraries (async
    ${CMAKE_THREAD_LIBS_INIT}
)




# секция создания deb пакета
# install (TARGETS async_cli RUNTIME DESTINATION bin)
install (TARGETS async     LIBRARY DESTINATION lib)

set (CPACK_GENERATOR DEB)

# эти установки обязательны, без них пакет не соберётся
# переменные версий установлены выше командой project
set (CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set (CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set (CPACK_PACKAGE_CONTACT slonkdv@mail.ru)

include (CPack)



enable_testing()

add_subdirectory(tests)