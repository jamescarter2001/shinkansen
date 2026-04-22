cmake_minimum_required(VERSION 4.1)
project(shinkansen)

set(CMAKE_CXX_STANDARD 20)

include_directories(shinkansen PRIVATE src)

add_executable(shinkansen src/main.cpp
        src/core/ring_buffer.h
        src/skpch.h
        src/util/sk_math.h
        src/core/shinkansen.h)

target_precompile_headers(shinkansen PRIVATE src/skpch.h)

### Test ###

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
        shinkansen_tests
        test/core/shinkansen_test.cpp
        test/core/ring_buffer_test.cpp
)
target_link_libraries(
        shinkansen_tests
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(shinkansen_tests)