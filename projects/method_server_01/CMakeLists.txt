cmake_minimum_required(VERSION 3.5)

project(simple_cliser)

SET(CMAKE_C_COMPILER	gcc)
SET(CMAKE_CXX_COMPILER	g++)

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set (OPEN62541_DIR ${PROJECT_SOURCE_DIR}/open62541)
set (OPEN62541_INCLUDE_DIR ${OPEN62541_DIR})
set (OPEN62541_LIB_DIR ${OPEN62541_DIR}/bin)

add_definitions(-std=c99)

include_directories(${OPEN62541_INCLUDE_DIR})
link_directories(${OPEN62541_LIB_DIR})

add_executable(method_server_01 ${PROJECT_SOURCE_DIR}/../src/method_server_01.c)
target_link_libraries(method_server_01 libopen62541.so)

add_executable(method_client_01 ${PROJECT_SOURCE_DIR}/../src/method_client_01.c)
target_link_libraries(method_client_01 libopen62541.so)
