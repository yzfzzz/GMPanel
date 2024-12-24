# �ҵ�ȫ�ֻ����е�grpc������
#[[
ָ��ʹ�� config ģʽ������ protobuf �⡣�� config ģʽ�£�CMake ��������Ϊ <package_name>-config.cmake 
�� config.cmake ���ļ������� protobuf ����˵������ protobuf-config.cmake �ļ�������Щ�ļ�ͨ���ɿ�İ�װ
�������ɣ������˿��ͷ�ļ�·�������ļ�·������Ϣ
]]

# Ϊʲô set(CMAKE_CXX_STANDARD 17) ?

find_package(protobuf CONFIG REQUIRED)
# find_package(gRPC CONFIG REQUIRED)
# find_package(c-ares CONFIG)


set(PROTO_FILES
    monitor_info.proto
    cpu_load.proto
    cpu_softirq.proto
    cpu_stat.proto
    mem_info.proto
    net_info.proto
    gpu_info.proto
    time.proto
    query.proto
)

# ������̬��
add_library(monitor_proto ${PROTO_FILES})
# ���Ӿ�̬��
target_link_libraries(monitor_proto
    PUBLIC
        protobuf::libprotobuf
        # gRPC::grpc
        # gRPC::grpc++
)
# ָ�����ͷ�ļ�·��
target_include_directories(monitor_proto PUBLIC
${PROTOBUF_INCLUDE_DIRS}
# ����о����Բ���
${CMAKE_CURRENT_BINARY_DIR}
)

# get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)
protobuf_generate(TARGET monitor_proto LANGUAGE cpp)
# protobuf_generate(TARGET monitor_proto LANGUAGE grpc GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}")

