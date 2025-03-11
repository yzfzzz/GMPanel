find_package(protobuf CONFIG REQUIRED)
if(RPC_TYPE STREQUAL "grpc")
find_package(gRPC CONFIG REQUIRED)
find_package(c-ares CONFIG)
endif()

set(PROTO_FILES
    monitor_info.proto
    cpu_load.proto
    cpu_softirq.proto
    cpu_stat.proto
    mem_info.proto
    net_info.proto
    gpu_info.proto
    hard_disk.proto
    os.proto
    time.proto
    query.proto
    login_register.proto
)
add_library(monitor_proto ${PROTO_FILES})

target_link_libraries(monitor_proto
    PUBLIC
        protobuf::libprotobuf
)

if(RPC_TYPE STREQUAL "grpc")
target_link_libraries(monitor_proto
    PUBLIC
        gRPC::grpc
        gRPC::grpc++
)
endif()

target_include_directories(monitor_proto PUBLIC
${PROTOBUF_INCLUDE_DIRS}
${CMAKE_CURRENT_BINARY_DIR}
)

if(RPC_TYPE STREQUAL "grpc")
    get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)
endif()
protobuf_generate(TARGET monitor_proto LANGUAGE cpp)
if(RPC_TYPE STREQUAL "grpc")
protobuf_generate(TARGET monitor_proto LANGUAGE grpc GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}")
endif()

