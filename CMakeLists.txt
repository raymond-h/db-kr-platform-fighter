cmake_minimum_required(VERSION 3.10)

include(ExternalProject)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

# set the project name
project(DbKrPlatformFighter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

enable_testing()
add_subdirectory("./test")

add_subdirectory("./dependencies")

# setup entt dependency
find_path(ENTT_INCLUDE_PATH entt/entt.hpp
  HINTS "${CMAKE_SOURCE_DIR}/dependencies/entt"
  PATH_SUFFIXES src
  NO_DEFAULT_PATH
)

# setup sdl2 dependency
find_package(SDL2 CONFIG REQUIRED)

# setup ggpo dependency
ExternalProject_Add(dependency_ggpo
    GIT_REPOSITORY https://github.com/pond3r/ggpo
    GIT_TAG 7ddadef8546a7d99ff0b3530c6056bc8ee4b9c0a
)
ExternalProject_Get_Property(dependency_ggpo BINARY_DIR)
set(GGPO_DIR ${BINARY_DIR}/installed)

file(GLOB SOURCES "src/*.cpp" "src/*.c")
set(DLLS "${GGPO_DIR}/bin/GGPO.dll")

# add the executable
add_executable(DbKrPlatformFighter ${SOURCES})
add_dependencies(DbKrPlatformFighter dependency_ggpo)
target_include_directories(DbKrPlatformFighter PRIVATE ${GGPO_DIR}/include ${ENTT_INCLUDE_PATH})
target_link_libraries(DbKrPlatformFighter Boost_Headers SDL2::SDL2 SDL2::SDL2main ${GGPO_DIR}/lib/GGPO.lib)

add_custom_command(TARGET DbKrPlatformFighter
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${DLLS}
        $<TARGET_FILE_DIR:DbKrPlatformFighter>
)

install(TARGETS DbKrPlatformFighter DESTINATION .)

get_target_property(SDL2_DLL SDL2::SDL2 IMPORTED_LOCATION_RELEASE)
install(FILES ${DLLS} "${SDL2_DLL}" DESTINATION .)

set(CPACK_GENERATOR "ZIP")
set(CPACK_PACKAGE_NAME "DbKrPlatformFighter")
include(CPack)
