## Dependencies
### Building
- Visual Studio 2017+
- CMake 3.10+

### Used by project (automatically downloaded)
- SDL2 (development files for Visual Studio)
- GGPO
- EnTT

## Setting up

This project uses CMake to manage dependencies and build the project.

## Building

Compile the project by whatever means you chose with cmake. Dependencies are downloaded and built automatically.

## Running

```sh
./build/Debug/DbKrPlatformFighter.exe
```

## Packaging

`[config]` should be replaced with any one of `Debug` or `Release` to do respective type of build.

```sh
cmake --build build/ --config [config] --target PACKAGE
```
