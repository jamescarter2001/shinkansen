cmake_minimum_required(VERSION 4.1)
project(shinkansen)

set(CMAKE_CXX_STANDARD 20)

include_directories(shinkansen PRIVATE src)

add_executable(shinkansen src/main.cpp
        src/core/ring_buffer.h
        src/skpch.h
        src/core/sk_math.h
        src/core/shinkansen.h)

target_precompile_headers(shinkansen PRIVATE src/skpch.h)