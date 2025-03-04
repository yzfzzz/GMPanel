set(SOURCES
    main.cpp
    rpc_manager.cpp
)

link_directories(${PROJECT_SOURCE_DIR}/build/proto ${PROJECT_SOURCE_DIR}/database/lib)
include_directories(${PROJECT_SOURCE_DIR}/build/proto ${PROJECT_SOURCE_DIR}/tool ${PROJECT_SOURCE_DIR}/database/include)

add_executable(server ${SOURCES})

target_link_libraries(server
    PUBLIC
    monitor_proto
    database
    glog
)




