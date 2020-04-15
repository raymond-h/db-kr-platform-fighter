## Dependencies
- CMake 3.10+
- SDL2 (development files for Visual Studio)
- GGPO
- EnTT

## Setting up

SDL2, EnTT and GGPO need to be downloaded beforehand and extracted and placed (location is up to you). Additionally, GGPO must be built beforehand using Visual Studio.

This project uses CMake to configure and generate files used for compilation.
A handful of config variables need to be set (use cmake-gui to easily manage them).

- `ENTT_SRC_PATH`: Path to `src/` directory in EnTT's root directory (`entt/entt.hpp` should be accessible from that folder)
- `GGPO_INCLUDE_PATH`: Path to directory containing `ggponet.h` in GGPO
- `GGPO_LIB_PATH`: Path to directory containing `GGPO.lib` (as a result of compiling GGPO)
- `SDL2_DIR`: Path to root folder of SDL2 - setting this will fill out the other `SDL2_...` configs automatically

## Running

Compile the project by whatever means you chose with cmake.

Ensure that `GGPO.dll` (if GGPO was built as a shared library) and `SDL2.dll` are on your `PATH`. A simple way to do it is to just copy those DLLs into the folder you're running the built program from.
