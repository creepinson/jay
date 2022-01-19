# jay

## Embed Using CMake

This is a minimal C++20 example that embeds jay into a cmake project.

```cmake
cmake_minimum_required(VERSION 3.22)
project(example VERSION 0.0.1 LANGUAGES CXX C)

# Modern c++ flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3 -g -Wpedantic -Wconversion -std=c++2b")

file(GLOB_RECURSE SOURCES src/*.cpp src/*.hpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC src)
target_link_libraries(${PROJECT_NAME} PUBLIC ${JAY_LIBRARIES})

find_library(JAY_LIBRARIES NAMES jay)
```
