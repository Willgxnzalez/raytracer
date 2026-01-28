cmake_minimum_required(VERSION 3.20)

project(raytracer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

file(GLOB_RECURSE SOURCES "src/*.cpp")

# Main executable
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
add_executable(raytracer src/main.cpp ${SOURCES})

include (FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

# Test executable
file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")
add_executable(raytracer_tests ${TEST_SOURCES} ${SOURCES})
target_link_libraries(raytracer_tests gtest_main)

# Discover tests
include(GoogleTest)
gtest_discover_tests(raytracer_tests)
