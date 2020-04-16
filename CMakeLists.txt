cmake_minimum_required(VERSION 3.10)

include(ExternalProject)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# set the project name
project(DbKrPlatformFighter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# setup entt dependency
find_path(ENTT_INCLUDE_PATH entt/entt.hpp
  HINTS "./dependencies/entt"
  PATH_SUFFIXES src
)

# setup sdl2 dependency
ExternalProject_Add(dependency_sdl
    URL https://www.libsdl.org/release/SDL2-devel-2.0.12-VC.zip
    URL_HASH SHA256=00C55A597CEBDB9A4EB2723F2AD2387A4D7FD605E222C69B46099B15D5D8B32D
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)
ExternalProject_Get_Property(dependency_sdl SOURCE_DIR)
find_path(SDL2_INCLUDE_DIRS SDL.h
    HINTS ${SOURCE_DIR}
    PATH_SUFFIXES include
)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(VC_LIB_PATH_SUFFIX lib/x64)
else()
    set(VC_LIB_PATH_SUFFIX lib/x86)
endif()
find_path(SDL2_LIB_PATH SDL2.lib
    HINTS ${SOURCE_DIR}
    PATH_SUFFIXES lib ${VC_LIB_PATH_SUFFIX}
)
set(SDL2_LIBRARIES ${SDL2_LIB_PATH}/SDL2.lib ${SDL2_LIB_PATH}/SDL2main.lib)

# setup ggpo dependency
ExternalProject_Add(dependency_ggpo
    GIT_REPOSITORY https://github.com/pond3r/ggpo
    GIT_TAG 7ddadef8546a7d99ff0b3530c6056bc8ee4b9c0a
)
ExternalProject_Get_Property(dependency_ggpo BINARY_DIR)
set(GGPO_DIR ${BINARY_DIR}/installed)

file(GLOB SOURCES "src/*.cpp")

# add the executable
add_executable(DbKrPlatformFighter ${SOURCES})
target_include_directories(DbKrPlatformFighter PRIVATE ${GGPO_DIR}/include ${SDL2_INCLUDE_DIRS} ${ENTT_INCLUDE_PATH})
target_link_libraries(DbKrPlatformFighter ${GGPO_DIR}/lib/GGPO.lib ${SDL2_LIBRARIES})

add_custom_command(TARGET DbKrPlatformFighter
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${GGPO_DIR}/bin/GGPO.dll"
        "${SDL2_LIB_PATH}/SDL2.dll"
        $<TARGET_FILE_DIR:DbKrPlatformFighter>
)

install(TARGETS DbKrPlatformFighter DESTINATION .)
install(FILES "${GGPO_DIR}/bin/GGPO.dll" "${SDL2_LIB_PATH}/SDL2.dll" DESTINATION .)

set(CPACK_GENERATOR "ZIP")
set(CPACK_PACKAGE_NAME "DbKrPlatformFighter")
include(CPack)
