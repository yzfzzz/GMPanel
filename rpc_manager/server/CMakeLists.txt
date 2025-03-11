# 查找依赖包
find_package(fmt REQUIRED)
find_package(yaml-cpp REQUIRED)

# 打印调试信息
message(STATUS "fmt include dir: ${fmt_INCLUDE_DIRS}")
message(STATUS "fmt libraries: ${fmt_LIBRARIES}")

# 定义源文件
set(SOURCES
    main.cpp
    rpc_manager.cpp
)

# 设置链接目录
link_directories(
    ${PROJECT_SOURCE_DIR}/build/proto
    ${PROJECT_SOURCE_DIR}/database/lib
)

# 设置包含目录
include_directories(
    ${PROJECT_SOURCE_DIR}/build/proto
    ${PROJECT_SOURCE_DIR}/utils
    ${PROJECT_SOURCE_DIR}/database/include
)

# 根据条件添加包含目录
if(RPC_TYPE STREQUAL "mprpc")
    link_directories(${PROJECT_SOURCE_DIR}/mprpc/lib)
    include_directories(${PROJECT_SOURCE_DIR}/mprpc/src/include)
endif()

# 创建可执行文件
add_executable(server ${SOURCES})

# 链接库
target_link_libraries(server
    PUBLIC
    fmt::fmt
    yaml-cpp
    monitor_proto
    database
    glog
)
# 根据条件链接库
if(RPC_TYPE STREQUAL "mprpc")
    target_link_libraries(server PUBLIC mprpc)
endif()