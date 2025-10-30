## Build

```
conan install . --build=missing -s build_type=Debug
conan install . --build=missing -s build_type=Release

cmake --preset conan-default

cmake --build ./build --config Debug
cmake --build ./build --config Release
```
