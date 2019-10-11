include_directories(
    ../include
    ../external
)

add_executable(threadpool_test
    threadpool_test.cpp
)

target_link_libraries(threadpool_test
    pthread
)