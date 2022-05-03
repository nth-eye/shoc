cmake_minimum_required(VERSION 3.0.0)
project(shoc VERSION 0.1.0)

add_library(shoclib INTERFACE)
target_compile_features(shoclib INTERFACE cxx_std_20)
target_compile_options(shoclib INTERFACE "-Wall" "-Wextra" "-Wpedantic")
target_include_directories(shoclib INTERFACE inc)

add_executable(shoc main.cpp)
target_compile_features(shoc PUBLIC cxx_std_20)
target_link_libraries(shoc PUBLIC shoclib)

enable_testing()
file(GLOB TEST_FILES test/*.cpp) 

add_executable(shoctest ${TEST_FILES})
target_compile_features(shoctest PUBLIC cxx_std_20)
target_link_libraries(shoctest PUBLIC gtest_main shoclib)

include(FetchContent)
FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)
gtest_discover_tests(shoctest)