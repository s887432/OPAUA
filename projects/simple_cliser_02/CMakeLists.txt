cmake_minimum_required(VERSION 3.5)

project(demoOpen62541)

#SET(CMAKE_C_COMPILER   /home/user/WorkArea/buildroot_a5d27_som1_opcua-2023.10/buildroot-at91/output/host/bin/arm-buildroot-linux-gnueabihf-gcc)
#SET(CMAKE_CXX_COMPILER /home/user/WorkArea/buildroot_a5d27_som1_opcua-2023.10/buildroot-at91/output/host/bin/arm-buildroot-linux-gnueabihf-g++)

SET(CMAKE_C_COMPILER   gcc)
SET(CMAKE_CXX_COMPILER g++)

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set (OPEN62541_DIR ${PROJECT_SOURCE_DIR}/open62541)
set (OPEN62541_INCLUDE_DIR ${OPEN62541_DIR})
set (OPEN62541_LIB_DIR ${OPEN62541_DIR}/bin)

add_definitions(-std=c99)

include_directories(${OPEN62541_INCLUDE_DIR})
link_directories(${OPEN62541_LIB_DIR})

add_executable(simple_server_02 ${PROJECT_SOURCE_DIR}/../src/simple_server_02.c)
target_link_libraries(simple_server_02 libopen62541.so)

add_executable(simple_clientv2 ${PROJECT_SOURCE_DIR}/../src/simple_client_02.c)
target_link_libraries(simple_client_02 libopen62541.so)	
