cmake_minimum_required(VERSION 3.0.0)
project(Server VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

add_executable(Server main.c server.c io.c)

target_link_libraries(Server Threads::Threads)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
