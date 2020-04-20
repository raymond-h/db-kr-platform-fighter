cmake_minimum_required(VERSION 3.10)

include(ExternalProject)

# setup ggpo dependency
ExternalProject_Add(dependency_catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2
    GIT_TAG 37cbf4a4fe7a2963f8ae6d6fe998f8ac9360cbc0
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)
ExternalProject_Get_Property(dependency_catch2 SOURCE_DIR)
set(CATCH2_INCLUDE_DIR ${SOURCE_DIR}/single_include)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${SOURCE_DIR}/contrib/")

file(GLOB TEST_SOURCES "*.cpp")

add_executable(TestExec ${TEST_SOURCES})
add_dependencies(TestExec dependency_catch2)
target_include_directories(TestExec PRIVATE "${CATCH2_INCLUDE_DIR}" "../src")

include(CTest)
include(Catch)
catch_discover_tests(TestExec)
