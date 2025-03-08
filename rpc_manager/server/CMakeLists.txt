find_package(fmt REQUIRED)
find_package(yaml-cpp REQUIRED)
message(STATUS "fmt include dir: ${fmt_INCLUDE_DIRS}")
message(STATUS "fmt libraries: ${fmt_LIBRARIES}")
set(SOURCES
    main.cpp
    rpc_manager.cpp
)

link_directories(${PROJECT_SOURCE_DIR}/build/proto ${PROJECT_SOURCE_DIR}/database/lib)
include_directories(${PROJECT_SOURCE_DIR}/build/proto ${PROJECT_SOURCE_DIR}/utils ${PROJECT_SOURCE_DIR}/database/include)

add_executable(server ${SOURCES})

target_link_libraries(server
    PUBLIC
    fmt::fmt
    yaml-cpp
    monitor_proto
    database
    glog
)




