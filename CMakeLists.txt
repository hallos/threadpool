cmake_minimum_required(VERSION 3.10)
set(CXX_STANDARD 14)
enable_testing()

include_directories(
    external
    include
)

add_subdirectory(test)