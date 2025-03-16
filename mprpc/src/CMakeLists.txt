find_package(yaml-cpp REQUIRED)
aux_source_directory(. SRC_LIST)
# 生成一个库
add_library(mprpc ${SRC_LIST})
# muduo库是静态库
target_link_libraries(mprpc muduo_net muduo_base pthread glog zookeeper_mt yaml-cpp)