cmake_minimum_required (VERSION 3.2)

function(test name)
    add_executable (${name} ${name}.cpp)
    target_link_libraries (${name} 
        ${Boost_LIBRARIES}
    )
    set_target_properties (${name} PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        COMPILE_OPTIONS -Wpedantic -Wall -Wextra
        COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
        INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
        INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}
    )
    add_test(${name} ../build/${name})
endfunction(test)


test(print_ip_char_test)
test(print_ip_short_test)
test(print_ip_int_test)
test(print_ip_long_test)
test(print_ip_string_test)
test(print_ip_conteiner_test)
test(print_ip_tuple_test)