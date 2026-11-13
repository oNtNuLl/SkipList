# 1. 规定 CMake 最低版本要求
cmake_minimum_required(VERSION 3.10)

# 2. 定义你的工程名字
project(SkipKV)

# 3. 指定 C++ 标准（咱们用 C++11 的特性就够了）
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 1. 寻找系统中的 Protobuf 库和编译器
find_package(Protobuf REQUIRED)
# 2. 告诉编译器去哪里找 Protobuf 的头文件以及自动生成的 .pb.h 文件
include_directories(include/)
include_directories(include/proto/pb)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# 3. 🌟 核心：让 CMake 自动根据 .proto 文件生成 C++ 源码
#protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS include/proto/NetWork.proto)

# 5. 生成可执行程序（最小化原则：坚决不自动扫描，有几个文件就手写几个）

add_custom_command(
        OUTPUT
        ${CMAKE_SOURCE_DIR}/include/proto/pb/NetWork.pb.h
        ${CMAKE_SOURCE_DIR}/include/proto/pb/NetWork.pb.cc
        COMMAND protoc
        --cpp_out=${CMAKE_SOURCE_DIR}/include/proto/pb
        --proto_path=${CMAKE_SOURCE_DIR}/include/proto
        NetWork.proto
        DEPENDS ${CMAKE_SOURCE_DIR}/include/proto/NetWork.proto
        COMMENT "Generating Protobuf C++ files to include/proto/pb/"
)
add_executable(skipkv_server
        src/main.cpp
        src/SkipList.cpp
        include/SkipList.h
        include/WALBuffer.cpp
        include/WALBuffer.h
        include/WALIO.cpp
        include/WALIO.h
        include/WALManage.cpp
        include/WALManage.h
        include/server/server.cpp
        include/server/server.h
        include/proto/pb/NetWork.pb.cc
        include/proto/pb/NetWork.pb.h
)
target_link_libraries(skipkv_server pthread ${Protobuf_LIBRARIES})