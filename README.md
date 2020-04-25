## Dependencies
### Building
- Visual Studio 2017+
- CMake 3.10+
- vcpkg 2020.04 or later

### Used by project (automatically downloaded)
- Boost
- SDL2
- GGPO
- EnTT

## Setting up

This project uses CMake to build the project and (mostly) vcpkg to manage dependencies. Ensure that vcpkg has been set up on your PC (AKA git clone and bootstrap it), and set environment variable `VCPKG_ROOT` to the root of the cloned vcpkg repo (the same folder you'll find the built program `vcpkg.exe`).

## Building

Compile the project by whatever means you chose with cmake. Dependencies are downloaded and built automatically if vcpkg is installed and set up.

## Running

```sh
./build/Debug/DbKrPlatformFighter.exe
```

## Packaging

`[config]` should be replaced with any one of `Debug` or `Release` to do respective type of build.

```sh
cmake --build build/ --config [config] --target PACKAGE
```

Alternatively, build the `INSTALL` target from your IDE.
