cmake_minimum_required(VERSION 3.0.0)
project(shoc VERSION 0.1.0)

add_subdirectory(lib/utl)

add_library(libshoc INTERFACE)
target_include_directories(libshoc INTERFACE inc)
target_compile_features(libshoc INTERFACE cxx_std_20)
target_compile_options(libshoc INTERFACE "-Wall" "-Wextra" "-Wpedantic")
target_link_libraries(libshoc INTERFACE libutl)

add_executable(shoc main.cpp)
target_link_libraries(shoc PRIVATE libshoc)

add_executable(testshoc 
    test/cipher/aes.cpp
    test/ecc/crc.cpp
    test/hash/hash.cpp
    test/kdf/hkdf.cpp
    test/mac/hmac.cpp
    test/mode/mode.cpp
    test/otp/hotp.cpp
    test/elliptic.cpp)
target_link_libraries(testshoc PRIVATE gtest_main libshoc)
