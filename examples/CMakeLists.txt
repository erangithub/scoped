cmake_minimum_required(VERSION 3.0)
project(scoped_tests)
include(CTest)
set(CMAKE_CXX_STANDARD 17)
include_directories(../include)

# Create the test driver list of tests
set (Examples 
    ex0_simple
    ex1_no_singleton
    ex2_caching
    ex3_event_counter
    ex4_decorator
    ex5_dependency_injection
)

# Add all the ADD_TEST for each test
foreach (test ${Examples})
    add_executable(${test}.exe ${test}.cpp)
endforeach()
