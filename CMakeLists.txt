#CMake最低版本号要求
cmake_minimum_required(VERSION 3.16)

#指定项目名称
project(demo)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#指定编译选项
set(CMAKE_BUILD_TYPE Debug)

#并将名称保存到DIR_LIB_SRCS目录
aux_source_directory(main.cpp DIR_LIB_SRCS)

#指定头文件目录,PROJECT_SOURCE_DIR为工程的根目录  
include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

aux_source_directory(src DIR_LIBS_SRCS)

#设置编译选项(会将gcc g++全部加上)
add_compile_options(-g -std=c++2a)
# add_compile_options(-fPIC)

add_compile_options(-fno-elide-constructors)    #关闭编译器优化

set(CMAKE_CXX_FLAGS "-Wall --std=c++2a -O0 -g")


#指定生成目标
add_executable(demo main.cpp ${DIR_LIBS_SRCS})



find_package(Threads)
target_link_libraries(demo ${CMAKE_THREAD_LIBS_INIT})
