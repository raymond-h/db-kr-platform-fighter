cmake_minimum_required(VERSION 3.10)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# set the project name
project(DbKrPlatformFighter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(GGPO_LIB_PATH "." CACHE PATH "Path to .lib file of GGPO")
set(GGPO_INCLUDE_PATH "." CACHE PATH "Path to header files of GGPO")
set(ENTT_SRC_PATH "." CACHE PATH "Path to header file of EnTT")

file(GLOB SOURCES "src/*.cpp")

find_package(SDL2 REQUIRED)

# add the executable
add_executable(DbKrPlatformFighter ${SOURCES})
target_include_directories(DbKrPlatformFighter PRIVATE ${SDL2_INCLUDE_DIRS} ${GGPO_INCLUDE_PATH} ${ENTT_SRC_PATH})
target_link_directories(DbKrPlatformFighter PRIVATE ${GGPO_LIB_PATH})
target_link_libraries(DbKrPlatformFighter ${SDL2_LIBRARIES} "GGPO.lib")
