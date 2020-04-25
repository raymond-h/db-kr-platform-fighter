cmake_minimum_required(VERSION 3.10)

find_package(Catch2 CONFIG REQUIRED)

file(GLOB TEST_SOURCES "*.cpp" "../src/fixed.cpp" "../src/aabb.cpp")

add_executable(TestExec ${TEST_SOURCES})
target_include_directories(TestExec PRIVATE "../src")
target_link_libraries(TestExec PRIVATE Boost::headers Catch2::Catch2)

include(CTest)
include(Catch)
catch_discover_tests(TestExec)
