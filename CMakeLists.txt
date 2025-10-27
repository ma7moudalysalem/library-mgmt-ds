cmake_minimum_required(VERSION 3.14)
project(LibraryMgmtDS LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(libmgmt
    src/library.cpp
)
target_include_directories(libmgmt PUBLIC include)

add_executable(app apps/cli/main.cpp)
target_link_libraries(app PRIVATE libmgmt)

enable_testing()
