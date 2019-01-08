cmake_minimum_required(VERSION 3.5)
project(yamr)

if ($ENV{TRAVIS_BUILD_NUMBER})
    project (yamr VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project (yamr VERSION 0.0.1)
endif()

find_package (Threads)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ../build)
# set (CMAKE_BUILD_TYPE Debug)

add_executable(yamr main.cpp)

set_target_properties (yamr PROPERTIES
   CXX_STANDARD 14
   CXX_STANDARD_REQUIRED ON
   COMPILE_OPTIONS -Wpedantic -Wall -Wextra
   COMPILE_FLAGS -O2
)


target_link_libraries (yamr
   ${CMAKE_THREAD_LIBS_INIT}
)




# секция создания deb пакета
install (TARGETS yamr RUNTIME DESTINATION bin)

set (CPACK_GENERATOR DEB)

# эти установки обязательны, без них пакет не соберётся
# переменные версий установлены выше командой project
set (CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set (CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set (CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set (CPACK_PACKAGE_CONTACT slonkdv@mail.ru)

include (CPack)



enable_testing()

add_subdirectory(tests)



