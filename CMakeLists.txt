cmake_minimum_required(VERSION 3.10)
project(linux_monitor LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

# set(CMAKE_BUILD_TYPE "Debug")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(RPC_TYPE "mprpc")

add_subdirectory(proto)

if(RPC_TYPE STREQUAL "mprpc")
    add_subdirectory(mprpc)
endif()

add_subdirectory(database)
add_subdirectory(rpc_manager/server)
add_subdirectory(rpc_manager/client)
# add_subdirectory(test_monitor)
# add_subdirectory(display_monitor)
add_subdirectory(http)