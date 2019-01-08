cmake_minimum_required (VERSION 3.2)

function(test name)
    add_executable (${name} ${name}.cpp)
    target_link_libraries (${name} 
        ${Boost_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
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


# тесты 7 задания
test(homework_test)
test(file_test)
test(cout_test)
test(exporter_test)
test(additional_parser_test)

# тесты 10 задания
test(thread_test)
test(out_of_memory_test)
test(efficiency_test)
test(benchmark_test)
test(cpu_load_test)

