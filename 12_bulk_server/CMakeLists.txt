cmake_minimum_required(VERSION 3.5)
project(bulk_server)

if ($ENV{TRAVIS_BUILD_NUMBER})
    project (bulk_server VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project (bulk_server VERSION 0.0.1)
endif()

find_package (Threads)
find_package (Boost COMPONENTS system REQUIRED)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ../build)
set (CMAKE_BUILD_TYPE Debug)

add_executable(bulk_server main.cpp ./otus10/threadsubs.cpp)

include_directories(bulk_server ${Boost_INCLUDE_DIRS})

set_target_properties (bulk_server PROPERTIES
   CXX_STANDARD 14
   CXX_STANDARD_REQUIRED ON
   COMPILE_OPTIONS -Wpedantic -Wall -Wextra
   COMPILE_FLAGS -O2
   COMPILE_DEFINITIONS BOOST_ASIO_ENABLE_HANDLER_TRACKING
   INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/otus10
)


target_link_libraries (bulk_server
   ${Boost_LIBRARIES}
   ${CMAKE_THREAD_LIBS_INIT}
)




# секция создания deb пакета
install (TARGETS bulk_server RUNTIME DESTINATION bin)

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



