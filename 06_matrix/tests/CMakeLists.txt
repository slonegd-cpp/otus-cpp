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


test(matrix_test)
test(homework_test)
test(bad_ptr_test)
test(types_test)
test(speed_test)
