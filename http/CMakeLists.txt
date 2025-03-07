find_package(fmt REQUIRED)
set(SOURCES
   http_server.cpp
   ../tool/query_data.cpp
)

find_package(Boost REQUIRED COMPONENTS system chrono date_time)

link_directories(${PROJECT_SOURCE_DIR}/build/proto 
                 ${PROJECT_SOURCE_DIR}/build/rpc_manager/client
                 ${PROJECT_SOURCE_DIR}/database/lib)

add_executable(http_server ${SOURCES})

target_include_directories(http_server PUBLIC
  ${PROJECT_SOURCE_DIR}/http
  ${PROJECT_SOURCE_DIR}/rpc_manager
  ${PROJECT_SOURCE_DIR}/database/include
  ${PROJECT_SOURCE_DIR}/build/proto
  ${PROJECT_SOURCE_DIR}/tool
)

target_link_libraries(http_server
    PUBLIC
    database
    monitor_proto
    client
    muduo_http
    muduo_net 
    muduo_base 
    pthread
    glog
    fmt::fmt
)