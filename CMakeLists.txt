cmake_minimum_required(VERSION 3.0.0)
project(shoc VERSION 0.1.0)

add_library(libshoc INTERFACE)
target_include_directories(libshoc INTERFACE inc)
target_compile_features(libshoc INTERFACE cxx_std_20)
target_compile_options(libshoc INTERFACE "-Wall" "-Wextra" "-Wpedantic")

add_executable(shoc main.cpp)
target_link_libraries(shoc PRIVATE libshoc)

add_executable(shoctest 
    test/test_aes.cpp
    test/test_hash.cpp
    test/test_hkdf.cpp
    test/test_hmac.cpp
    test/test_hotp.cpp
    test/test_mode.cpp)
target_link_libraries(shoctest PRIVATE gtest_main libshoc)

enable_testing()
include(FetchContent)
FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)
gtest_discover_tests(shoctest)
