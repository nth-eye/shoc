cmake_minimum_required(VERSION 3.0.0)
project(creep VERSION 0.1.0)

# set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")

add_library(creeplib INTERFACE)
target_compile_features(creeplib INTERFACE cxx_std_20)
target_include_directories(creeplib INTERFACE inc)

add_executable(creep main.cpp)
target_compile_features(creep PUBLIC cxx_std_20)
target_link_libraries(creep PUBLIC creeplib)

enable_testing()
file(GLOB TEST_FILES test/*.cpp) 

add_executable(creeptest ${TEST_FILES})
target_compile_features(creeptest PUBLIC cxx_std_20)
target_link_libraries(creeptest PUBLIC gtest_main creeplib)

include(FetchContent)
FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)
gtest_discover_tests(creeptest)