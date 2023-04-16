cmake_minimum_required(VERSION 3.0)
project(scoped_tests)
include(CTest)
set(CMAKE_CXX_STANDARD 17)
include_directories(../include)

# Create the test driver list of tests
set (Tests 
    test_simple
    test_vector 
    test_move
    test_option
)

# Add all the ADD_TEST for each test
foreach (test ${Tests})
    add_executable(${test}.exe ${test}.cpp)
    
    add_test(NAME ${test} COMMAND ${test}.exe)
endforeach()
