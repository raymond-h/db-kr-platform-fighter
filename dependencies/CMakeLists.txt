cmake_minimum_required(VERSION 3.10)

include(ExternalProject)

# setup boost (header-only) dependency
ExternalProject_Add(dependency_boost
    URL https://dl.bintray.com/boostorg/release/1.72.0/source/boost_1_72_0.zip
    URL_HASH SHA256=8c20440aaba21dd963c0f7149517445f50c62ce4eb689df2b5544cc89e6e621e
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)
ExternalProject_Get_Property(dependency_boost SOURCE_DIR)
set(BOOST_ROOT "${SOURCE_DIR}")
add_library(Boost_Headers INTERFACE)
target_include_directories(Boost_Headers INTERFACE "${BOOST_ROOT}")
