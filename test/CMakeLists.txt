cmake_minimum_required(VERSION 3.10)

include(FetchContent)
include(ExternalProject)

# setup ggpo dependency
FetchContent_Declare(catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2
    GIT_TAG 37cbf4a4fe7a2963f8ae6d6fe998f8ac9360cbc0
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)
FetchContent_MakeAvailable(catch2)
set(CATCH2_INCLUDE_DIR ${catch2_SOURCE_DIR}/single_include)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${catch2_SOURCE_DIR}/contrib/")

file(GLOB TEST_SOURCES "*.cpp" "../src/fixed.cpp")

add_executable(TestExec ${TEST_SOURCES})
target_include_directories(TestExec PRIVATE "${CATCH2_INCLUDE_DIR}" "../src")
target_link_libraries(TestExec PRIVATE Boost_Headers)

include(CTest)
include(Catch)
catch_discover_tests(TestExec)
