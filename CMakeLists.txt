cmake_minimum_required(VERSION 3.22)
project(jay VERSION 0.0.1 LANGUAGES CXX)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.h)
file(GLOB_RECURSE TEST_SOURCES test/*.cpp test/*.h)

# Modern c++ flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3 -g -Wpedantic -Wconversion -std=c++2b")

add_library(
    ${PROJECT_NAME}
    ${SOURCES}
)

find_library(FMT_LIBRARIES NAMES fmt)
find_library(SPDLOG_LIBRARIES NAMES spdlog)
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    ${FMT_LIBRARIES}
    ${SPDLOG_LIBRARIES}
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>
)
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER src/jay.h)
install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}-targets
    PUBLIC_HEADER DESTINATION include
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)


add_executable(
    ${PROJECT_NAME}_test
    ${TEST_SOURCES}
)
find_package(doctest REQUIRED)
target_link_libraries(
    ${PROJECT_NAME}_test
    ${PROJECT_NAME}
    doctest::doctest
)

# clang-format and clang-tidy
include(cmake/cxx-dev-tools.cmake)

