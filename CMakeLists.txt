cmake_minimum_required(VERSION 3.18)
project(limos)

set(SOURCES src/channel.c src/worker.c)
link_libraries(pthread php)
include_directories(include)

add_executable(${PROJECT_NAME} ${SOURCES} src/main.c)

enable_testing()

add_executable("test_channel" ${SOURCES} tests/test_channel.c)
add_test(NAME "test_channel" COMMAND test_channel)

add_executable("test_worker" ${SOURCES} tests/test_worker.c)
add_test(NAME "test_worker" COMMAND test_worker)
