set(SOURCES rpc_client.cpp)

link_directories(${PROJECT_SOURCE_DIR}/build/proto)
include_directories(${PROJECT_SOURCE_DIR}/build/proto ${PROJECT_SOURCE_DIR}/tool)

add_library(client ${SOURCES})

target_link_libraries(client 
    PUBLIC 
    monitor_proto)
