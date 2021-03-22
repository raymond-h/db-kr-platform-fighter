cmake_minimum_required(VERSION 3.10)

include(FetchContent)

execute_process(
    COMMAND "git" "submodule" "update" "--init"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

if(DEFINED ENV{VCPKG_ROOT})
    set(VCPKG_PACKAGES "sdl2" "boost-utility" "catch2")

    execute_process(COMMAND "$ENV{VCPKG_ROOT}/vcpkg" "install" ${VCPKG_PACKAGES})
endif()

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

# set the project name
project(DbKrPlatformFighter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# setup ggpo dependency
FetchContent_Declare(dependency_ggpo
    GIT_REPOSITORY https://github.com/pond3r/ggpo
    GIT_TAG 7ddadef8546a7d99ff0b3530c6056bc8ee4b9c0a
)
FetchContent_GetProperties(dependency_ggpo)
if(NOT dependency_ggpo_POPULATED)
  FetchContent_Populate(dependency_ggpo)

  add_subdirectory(${dependency_ggpo_SOURCE_DIR} ${dependency_ggpo_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# setup boost dependency
find_package(Boost REQUIRED)

# setup entt dependency
find_path(ENTT_INCLUDE_PATH entt/entt.hpp
  HINTS "${CMAKE_SOURCE_DIR}/dependencies/entt"
  PATH_SUFFIXES src
  NO_DEFAULT_PATH
)

# setup sdl2 dependency
find_package(SDL2 CONFIG REQUIRED)

# setup sdl-gpu
add_subdirectory("./dependencies/sdl-gpu")

# setup imgui
file(GLOB IMGUI_SOURCES "dependencies/imgui/*.cpp")
add_library(ImGui OBJECT ${IMGUI_SOURCES})
target_include_directories(ImGui PUBLIC "dependencies/imgui" "dependencies/imgui/backends")

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c")

# add the executable
add_executable(DbKrPlatformFighter ${SOURCES} "dependencies/imgui/backends/imgui_impl_sdl.cpp" "dependencies/imgui/backends/imgui_impl_opengl3.cpp")
target_include_directories(DbKrPlatformFighter
    PRIVATE
        ${SDL2_INCLUDE_DIR}
        ${ENTT_INCLUDE_PATH}
        ${SDL_gpu_SOURCE_DIR}/include
        "dependencies/sdl-gpu/src/externals/glew"
)
target_link_libraries(DbKrPlatformFighter PRIVATE Boost::headers GGPO ImGui SDL_gpu opengl32 glu32)

add_custom_command(TARGET DbKrPlatformFighter
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:GGPO>
        $<TARGET_FILE:SDL2::SDL2>
        $<TARGET_FILE_DIR:DbKrPlatformFighter>
)

install(TARGETS DbKrPlatformFighter DESTINATION .)
install(FILES $<TARGET_FILE:GGPO> $<TARGET_FILE:SDL2::SDL2> DESTINATION .)

set(CPACK_GENERATOR "ZIP")
set(CPACK_PACKAGE_NAME "DbKrPlatformFighter")
include(CPack)

enable_testing()
add_subdirectory("./test")
