cmake_minimum_required (VERSION 3.5)

find_package (Boost COMPONENTS unit_test_framework REQUIRED)

include_directories (
   ${PROJECT_SOURCE_DIR}
   ${Boost_INCLUDE_DIR}
)

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
   )
   add_test(${name} ../build/${name})
endfunction(test)



test(01_split_test)
test(02_mapper_test)
test(03_reducer_test)
test(04_homework_test)