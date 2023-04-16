# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -fcoroutines -std=c++20")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# 项目信息
project (Demo1)

# 指定生成目标
add_executable(Demo ReturnObject3.cpp)