# Проверка версии CMake.
# Если версия установленой программы
# старее указаной, произайдёт аварийный выход.
cmake_minimum_required (VERSION 3.2)

# задать название и версию пакета
# условие, чтобы собирался дома
if ($ENV{TRAVIS_BUILD_NUMBER})
    project (helloworld VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project (helloworld VERSION 0.0.1)
endif()

find_package (Boost COMPONENTS unit_test_framework REQUIRED)

# так как cmake работает в отдельном каталоге, появилось ../
configure_file(version_def.h.in ../version_def.h)

# список исходников, может быть несколько
# разделять пробелами переносами табами
# set (SOURCE main.cpp)
# вариант со сбором файлов по маске
#file (GLOB SOURCE "*.cpp")
#file (GLOB SOURCE_TEST "./test/*.cpp")

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ../build)

# Создает исполняемый файл из исходников
add_executable (helloworld main.cpp version.cpp)
add_executable (helloworld_test version_test.cpp version.cpp)
target_link_libraries (helloworld_test
    ${Boost_LIBRARIES}
)

# Задаёт опции компиляции
set_target_properties (helloworld helloworld_test PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)
set_target_properties (helloworld_test PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)



# секция создания deb пакета
install (TARGETS helloworld RUNTIME DESTINATION bin)

set (CPACK_GENERATOR DEB)

# эти установки обязательны, без них пакет не соберётся
# переменные версий установлены выше командой project
set (CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set (CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set (CPACK_PACKAGE_CONTACT slonkdv@mail.ru)

include (CPack)



enable_testing()

add_test(version_test ../build/helloworld_test)