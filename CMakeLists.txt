#设置CMake的最低版本要求
cmake_minimum_required(VERSION 3.10)
#设置项目名称
project(test)
#查找pthread库，并将其链接到程序中
find_package(Threads REQUIRED)
#添加可执行文件
add_executable(test_threadpool test_threadpool.cpp)
#将pthread库链接到可执行文件
target_link_libraries(test_threadpool PRIVATE Threads::Threads)