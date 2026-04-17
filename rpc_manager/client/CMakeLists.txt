find_package(fmt REQUIRED)
set(SOURCES rpc_client.cpp)

link_directories(${PROJECT_SOURCE_DIR}/build/proto)

include_directories(${PROJECT_SOURCE_DIR}/build/proto 
    ${PROJECT_SOURCE_DIR}/utils)

if(RPC_TYPE STREQUAL "yrpc")
    link_directories(${PROJECT_SOURCE_DIR}/yrpc/lib)
    include_directories(${PROJECT_SOURCE_DIR}/yrpc/src/include)
endif()

add_library(client ${SOURCES})

target_link_libraries(client 
    PUBLIC 
    monitor_proto
    fmt::fmt)
