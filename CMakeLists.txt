# 1. 规定 CMake 最低版本要求
cmake_minimum_required(VERSION 3.10)

# 2. 定义你的工程名字
project(SkipKV)

# 3. 指定 C++ 标准（咱们用 C++11 的特性就够了）
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 4. 告诉编译器去哪里找头文件 (不然你的 #include "SkipList.h" 会报错)
include_directories(include)

# 5. 生成可执行程序（最小化原则：坚决不自动扫描，有几个文件就手写几个）
add_executable(skipkv_server
        src/main.cpp
        src/SkipList.cpp
        include/SkipList.h
)