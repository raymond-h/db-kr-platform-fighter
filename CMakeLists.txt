cmake_minimum_required(VERSION 3.10)

# set the project name
project(DbKrPlatformFighter)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(GGPO_LIB_PATH "." CACHE PATH "Path to .lib file of GGPO")
set(GGPO_INCLUDE_PATH "." CACHE PATH "Path to header files of GGPO")

file(GLOB SOURCES "src/*.cpp")

# add the executable
add_executable(DbKrPlatformFighter ${SOURCES})
target_include_directories(DbKrPlatformFighter PRIVATE ${GGPO_INCLUDE_PATH})
target_link_directories(DbKrPlatformFighter PRIVATE ${GGPO_LIB_PATH})
target_link_libraries(DbKrPlatformFighter "GGPO.lib")
