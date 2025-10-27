cmake_minimum_required(VERSION 3.14)
project(LibraryMgmtDS LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Library (production code) ---
add_library(libmgmt
    src/library.cpp
)
target_include_directories(libmgmt PUBLIC include)

# --- CLI app ---
add_executable(app apps/cli/main.cpp)
target_link_libraries(app PRIVATE libmgmt)

# --- Tests (Catch2 v3 via FetchContent) ---
include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.6.0
)
FetchContent_MakeAvailable(Catch2)

add_executable(libmgmt_tests
  tests/test_avl.cpp
  tests/test_queue_stack.cpp
  tests/test_binary.cpp
  tests/test_library.cpp
)
target_link_libraries(libmgmt_tests PRIVATE libmgmt Catch2::Catch2WithMain)

include(CTest)
enable_testing()
add_test(NAME libmgmt_tests COMMAND libmgmt_tests)
