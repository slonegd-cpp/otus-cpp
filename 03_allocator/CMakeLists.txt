cmake_minimum_required (VERSION 3.2)

# задать название и версию пакета
# условие, чтобы собирался дома
if ($ENV{TRAVIS_BUILD_NUMBER})
    project (allocator VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project (allocator VERSION 0.0.1)
endif()

find_package (Boost COMPONENTS unit_test_framework REQUIRED)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ../build)

# Создает исполняемый файл из исходников
add_executable (allocator main.cpp newdelete.cpp)


# Задаёт опции компиляции
set_target_properties (allocator PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)


# секция создания deb пакета
install (TARGETS allocator RUNTIME DESTINATION bin)

set (CPACK_GENERATOR DEB)

# эти установки обязательны, без них пакет не соберётся
set (CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set (CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set (CPACK_PACKAGE_CONTACT slonkdv@mail.ru)

include (CPack)

enable_testing()

add_subdirectory(tests)

